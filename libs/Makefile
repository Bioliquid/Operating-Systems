package parser;

import expression.Expression;
import expression.Implication;
import expression.Variable;

import java.io.*;
import java.util.*;

public class Deductor {
private HashSet<Expression> hypotheses = new HashSet<>();
private HashSet<Expression> proved = new HashSet<>();
private HashMap<Expression, Expression> deduct = new HashMap<>();
private HashMap<Expression, List<Expression» toDeduct = new HashMap<>();
private HashMap<String, Expression> sub = new HashMap<>();

private Expression[] axioms = new Expression[]{
ExpressionParser.getExp("A->B->A"),
ExpressionParser.getExp("(A->B)->(A->B->C)->(A->C)"),
ExpressionParser.getExp("A->B->A&B"),
ExpressionParser.getExp("(A&B)->A"),
ExpressionParser.getExp("(A&B)->B"),
ExpressionParser.getExp("A->(A|B)"),
ExpressionParser.getExp("B->(A|B)"),
ExpressionParser.getExp("(A->C)->(B->C)->((A|B)->C)"),
ExpressionParser.getExp("(A->B)->(A->(!B))->(!A)"),
ExpressionParser.getExp("(!!A)->A")
};

private int check(Expression ex) {
for (int i = 0; i < 10; i++) {
sub.clear();
if (checkAxiom(axioms[i], ex)) return i + 1;
}
return 0;
}

private boolean checkAxiom(Expression ax, Expression ex) {
if (ax instanceof Variable) {
Variable a = (Variable) ax;
if (sub.containsKey(a.getName())) {
return sub.get(a.getName()).equals(ex);
} else {
sub.put(a.getName(), ex);
return true;
}
} else {
if (!ax.getOperation().equals(ex.getOperation())) {
return false;
}
Expression[] first = ax.getArguments(), second = ex.getArguments();
for (int i = 0; i < first.length; i++) {
if (!checkAxiom(first[i], second[i])) {
return false;
}
}
return true;
}
}

private void writeHypotheseOrAxiom(Expression a, Expression e, BufferedWriter w) throws IOException {
String ee = e.toString();
w.write("(" + ee + "->(" + a.toString() + "->" + ee + "))\n" + ee + "\n");
}

private void writeAToA(Expression a, BufferedWriter w) throws IOException {
String s = "(" + a.toString() + ")";
w.write("(" + s + "->(" + s + "->" + s + "))\n");
w.write("(" + s + "->((" + s + "->" + s + ")->" + s + "))\n");
w.write("((" + s + "->(" + s + "->" + s + "))->((" + s + "->((" + s + "->" + s + ")->" + s + "))->(" + s + "->" + s + ")))\n");
w.write("((" + s + "->((" + s + "->" + s + ")->" + s + "))->(" + s + "->" + s + "))\n");
}

private void writeMP(Expression left, Expression right, Expression alpha, BufferedWriter w) throws IOException {
String l = "(" + left.toString() + ")", r = "(" + right.toString() + ")", a = "(" + alpha.toString() + ")";
w.write("(" + a + "->" + l + ")->((" + a + "->(" + l + "->" + r + "))->(" + a + "->" + r + "))\n");
w.write("((" + a + "->(" + l + "->" + r + "))->(" + a + "->" + r + "))\n");
}

public ArrayList<String> deduct(ArrayList<String> proof) throws Exception {
Iterator<String> it = proof.iterator();
String s = it.next();
ArrayList<String> out = new ArrayList<>();
String[] h = s.split("\\|-")[0].split(",");
String toProve = ExpressionParser.getExp(s.split("\\|-")[1]).toString();
for (int i = 0; i < h.length - 1; i++) {
out.write(getExp(h[i]).toString());
if (i != h.length - 2) out.write(",");
if (!h[i].equals("")) hypotheses.add(getExp(h[i]));
}
int count = 0;
Expression alpha = getExp(h[h.length - 1]);
out.write("|-(" + getExp(h[h.length - 1]).toString() + ")->(" + getExp(toProve).toString() + ")\n");
while ((s = in.readLine()) != null) {
s = removeWhitespaces(s);
if (s.isEmpty()) {
continue;
}
Expression ex = getExp(s);
if (hypotheses.contains(ex) || check(ex) != 0) {
writeHypotheseOrAxiom(alpha, ex, out);
} else if (ex.equals(alpha)) {
writeAToA(ex, out);
} else {
//System.out.println(ex.toString() + " " + deduct.containsKey(ex));
Expression left = deduct.get(ex).getArguments()[0];
writeMP(left, ex, alpha, out);
}
proved.add(ex);
out.write("(" + alpha.toString() + ")->(" + ex.toString() + ")\n");
if (ex instanceof Implication) {
Implication im = (Implication) ex;
if (proved.contains(im.getLeft())) {
deduct.put(im.getRight(), ex);
} else {
toDeduct.computeIfAbsent(im.getLeft(), k -> new ArrayList<>());
toDeduct.get(im.getLeft()).add(im.getRight());
}
}

if (toDeduct.containsKey(ex)) {
for (Expression e : toDeduct.get(ex)) {
deduct.put(e, getExp("(" + ex.toString() + ")->(" + e.toString() + ")"));
}
}
count++;
//out.write("\n");
}
}
}