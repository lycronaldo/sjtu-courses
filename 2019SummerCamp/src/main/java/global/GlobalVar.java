package global;

import org.junit.Test;

public class GlobalVar {

    public static void log(Object... info) {
        String className = Thread.currentThread().getStackTrace()[2].getClassName();
        String methodName = Thread.currentThread().getStackTrace()[2].getMethodName();
        String title = className + "." + methodName;
        System.out.printf("[%s]\t", title);
        for (int i = 0; i < info.length; i++) {
            System.out.print(info[i] + " ");
        }
        System.out.println();
    }
}
