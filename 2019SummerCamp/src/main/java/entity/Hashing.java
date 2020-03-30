package entity;

public interface Hashing {

    public void set(long key, long val);
    public void delete(long key);
    public Tuple get(long key);
}
