package ch1.sec3;

import edu.princeton.cs.algs4.StdIn;

import java.util.DoubleSummaryStatistics;
import java.util.Stack;

/**
 * Created by jeson on 2015/11/23.
 */
public class EvaluateDelux {
    public static void main(String[] args)
    {
        Stack<String> ops = new Stack<>();
        Stack<Double> vals = new Stack<>();
        while (!StdIn.isEmpty())
        {
            String s = StdIn.readString();
            if (s.equals("("))
            {
            }
            else if (s.equals("*") || s.equals("/"))
            {
                Double v = vals.pop();

            }
            else if (s.equals("+") || s.equals("-"))
            {
            }
            else
            {
                vals.push(Double.parseDouble(s));
            }
        }
    }
}
