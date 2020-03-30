package entity;


public class CuckooHashing implements Hashing {
    private static int CAPACITY = 8;
    private Tuple[] table1 = null;
    private Tuple[] table2 = null;

    public CuckooHashing() {
        table1 = new Tuple[CAPACITY];
        table2 = new Tuple[CAPACITY];
    }

    private int hash1(long key) {
        return (int) (key % CAPACITY);
    }

    private int hash2(long key) {
        return (int) ((key / 8) % CAPACITY);
    }

    public Tuple get(long key) {
        Tuple tuple1 = table1[hash1(key)];
        Tuple tuple2 = table2[hash2(key)];

        if (tuple1 != null && tuple1.getKey() == key) {
            return tuple1;
        }
        if (tuple2 != null && tuple2.getKey() == key) {
            return tuple2;
        }
        return null;
    }

    public void set(long key, long val) {
        Tuple tuple = get(key);
        if (tuple == null) {
            boolean[][] visited = new boolean[2][CAPACITY];
            tuple = new Tuple(key, val);
            // if first insert operation failed, then increase capacity and try again
            if (!innerSet2(tuple, true, visited)) {
                increaseCapacity();
                visited = new boolean[2][CAPACITY];
                innerSet2(tuple, true, visited);
            }
        } else {
            tuple.setVal(val);
        }
    }

    private boolean innerSet2(Tuple tuple, boolean isLeft, boolean[][] visited) {
        Tuple[] table = isLeft ? table1 : table2;
        int idx = isLeft ? hash1(tuple.getKey()) : hash2(tuple.getKey());
        int flag = isLeft ? 0 : 1;
        if (visited[flag][idx]) {
            return false;
        }
        visited[flag][idx] = true;
        if (table[idx] == null) {
            table[idx] = new Tuple(tuple);
            return true;
        } else {
            Tuple t = table[idx];
            if (innerSet2(t, !isLeft, visited)) {
                table[idx] = new Tuple(tuple);
                return true;
            }
        }
        return false;
    }

    /**
     * There is a simplified version, see method innerSet2 above.
     *
     * @param isLeft true means do something on table1
     */
    private boolean innerSet(Tuple tuple, boolean isLeft, boolean[][] visited) {
        if (isLeft) {
            int idx = hash1(tuple.getKey());
            if (visited[0][idx]) {
                return false;
            }
            visited[0][idx] = true;
            if (table1[idx] == null) {
                table1[idx] = new Tuple(tuple);
                return true;
            } else {
                Tuple t = table1[idx];
                if (innerSet(t, false, visited)) {
                    table1[idx] = new Tuple(tuple);
                    return true;
                }
            }
        } else {
            int idx = hash2(tuple.getKey());
            if (visited[1][idx]) {
                return false;
            }
            visited[1][idx] = true;
            if (table2[idx] == null) {
                table2[idx] = new Tuple(tuple);
                return true;
            } else {
                Tuple t = table2[idx];
                if (innerSet(t, true, visited)) {
                    table2[idx] = new Tuple(tuple);
                    return true;
                }
            }
        }
        return false;
    }


    public void delete(long key) {
        int idx1 = hash1(key);
        int idx2 = hash2(key);
        if (table1[idx1] != null && table1[idx1].getKey() == key) {
            table1[idx1] = null;
        }
        if (table2[idx2] != null && table2[idx2].getKey() == key) {
            table2[idx2] = null;
        }
    }

    public void increaseCapacity() {
        Tuple[] t1 = this.table1;
        Tuple[] t2 = this.table2;
        CAPACITY *= 2;
        table1 = new Tuple[CAPACITY];
        table2 = new Tuple[CAPACITY];
        for (Tuple e : t1) {
            if (e != null) {
                set(e.getKey(), e.getVal());
            }
        }
        for (Tuple e : t2) {
            if (e != null) {
                set(e.getKey(), e.getVal());
            }
        }
    }

}
