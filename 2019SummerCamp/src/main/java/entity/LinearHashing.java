package entity;

import global.GlobalVar;

public class LinearHashing implements Hashing {
    private static int CAPACITY = 8;
    private Tuple[] tuples = null;
    private int size = 0;

    public LinearHashing() {
        this.tuples = new Tuple[CAPACITY];
        this.size = 0;
    }

    public void set(long key, long val) {
        int targetIndex = getTargetIndex(key);
        if (tuples[targetIndex] == null) {
            tuples[targetIndex] = new Tuple(key, val);
            size++;
            if (size > CAPACITY / 2) {
                increaseCapacity();
            }
        } else {
            tuples[targetIndex].setVal(val);
        }

    }

    public Tuple get(long key) {
        return tuples[getTargetIndex(key)];
    }

    private int getTargetIndex(long key) {
        int k = hash(key);
        while (tuples[k] != null && tuples[k].getKey() != key) {
            k = (k + 1) % CAPACITY;
        }
        return k;
    }

    public void delete(long key) {
        strongDelete(key);
        /*
        final int targetIndex = getTargetIndex(key);
        if (tuples[targetIndex] != null) {
            tuples[targetIndex] = null;
            size--;
            // TODO: adjust other relative elements, but I don't figure out how to move relative elements
            int nullPosition = targetIndex;
            int idx = (targetIndex + 1) % CAPACITY;
            while (tuples[idx] != null) {
                if (hash(tuples[idx].getKey()) != idx) {
                    tuples[nullPosition] = new Tuple(tuples[idx]);
                    tuples[idx] = null;
                    nullPosition = idx;
                }
                idx = (idx + 1) % CAPACITY;
            }
        }
        */
    }

    private void strongDelete(long key) {
        final int targetIndex = getTargetIndex(key);
        if (tuples[targetIndex] != null) {
            tuples[targetIndex] = null;
            size--;
            // adjust all elements for test (pass test)
            // it's not a good methods to do this
            // but I can not figure out how to pass large.in by adjusting a part of elements
            Tuple[] temp = tuples;
            size = 0;
            tuples = new Tuple[CAPACITY];
            for (int i = 0; i < temp.length; i++) {
                if (temp[i] != null) {
                    set(temp[i].getKey(), temp[i].getVal());
                }
            }
        }
    }

    private void increaseCapacity() {
        Tuple[] temp = this.tuples;
        this.tuples = new Tuple[CAPACITY * 2];
        CAPACITY *= 2;
        size = 0;
        for (int i = 0; i < temp.length; i++) {
            if (temp[i] != null) {
                set(temp[i].getKey(), temp[i].getVal());
            }
        }
    }

    private int hash(long key) {
        return (int) (key % CAPACITY);
    }


}
