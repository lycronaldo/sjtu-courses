

import entity.CuckooHashing;
import entity.Hashing;
import entity.LinearHashing;
import entity.Tuple;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;

public class Main {
    public static void main(String[] args) throws Exception {
//        Hashing hashing = new LinearHashing();
        Hashing hashing = new CuckooHashing();
        runTest("small.in", hashing);
        runTest("large.in", hashing);
    }


    public static void runTest(String filename, Hashing hashing) throws Exception {

        String root = "src/main/resources/";
        BufferedReader reader = new BufferedReader(new FileReader(root + filename));
        BufferedWriter writer = new BufferedWriter(new FileWriter(root + filename.replace(".in", ".out")));

        String line = null;
        while ((line = reader.readLine()) != null) {
            String[] data = line.split("\\s+");
            if (data[0].equals("Set")) {
                long key = Long.parseLong(data[1]);
                long val = Long.parseLong(data[2]);
                hashing.set(key, val);

            } else if (data[0].equals("Get")) {
                long key = Long.parseLong(data[1]);
                Tuple tuple = hashing.get(key);
                if (tuple == null) {
                    writer.write("null\n");
                } else {
                    writer.write(tuple.getVal() + "\n");
                }
            } else if (data[0].equals("Del")) {
                long key = Long.parseLong(data[1]);
                hashing.delete(key);
            } else {
                throw new Exception("Unknown operation " + data[0]);
            }
        }

        reader.close();
        writer.flush();
        writer.close();
    }

}
