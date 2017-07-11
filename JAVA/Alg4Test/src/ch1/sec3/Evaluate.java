package ch1.sec3;

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Stack;

/**
 * Created by jeson on 2015/11/6.
 * ( 1 + ( ( 2 + 3 ) * ( 4 * 5 ) ) ) = 101.0
 * ( 1 + sqrt ( 5 ) ) / 2.0 ) = 1.618033988749895
 */
public class Evaluate {
    public static void main(String[] args)
    {
        Stack<String> ops = new Stack<>();
        Stack<Double> vals = new Stack<>();

        double val = 0;
        while (!StdIn.isEmpty()) {
            String s = StdIn.readString();
            if (s.equals("(")) ;
            else if (s.equals("+") || s.equals("-") || s.equals("*") || s.equals("/") || s.equals("sqrt"))
                ops.push(s);
            else if (s.equals(")")) {
                String op = ops.pop();
                Double v = vals.pop();
                if (op.equals("+")) v = vals.pop() + v;
                else if (op.equals("-")) v = vals.pop() - v;
                else if (op.equals("*")) v = vals.pop() * v;
                else if (op.equals("/")) v = vals.pop() / v;
                else if (op.equals("sqrt")) v = Math.sqrt(v);
                vals.push(v);
            } else {
                vals.push(Double.parseDouble(s));
            }
        }
        StdOut.println(vals.pop());

    }
}
