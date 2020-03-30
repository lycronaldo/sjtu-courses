package entity;

public class Tuple {
    private long key, val;

    public Tuple(long key, long val) {
        this.key = key;
        this.val = val;
    }

    public Tuple() {
        this.key = -1;
        this.val = -1;
    }

    public Tuple(Tuple tuple) {
        this.key = tuple.key;
        this.val = tuple.val;
    }

    public long getKey() {
        return key;
    }

    public long getVal() {
        return val;
    }

    public void setVal(long val) {
        this.val = val;
    }

}
