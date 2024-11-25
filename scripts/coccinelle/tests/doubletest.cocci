<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/// Find &&/|| operations that include the same argument more than once
//# A common source of false positives is when the expression, or
//# another expresssion in the same && or || operation, performs a
//# side effect.
///
// Confidence: Moderate
<<<<<<< HEAD
// Copyright: (C) 2010 Nicolas Palix, DIKU.  GPLv2.
// Copyright: (C) 2010 Julia Lawall, DIKU.  GPLv2.
// Copyright: (C) 2010 Gilles Muller, INRIA/LiP6.  GPLv2.
=======
// Copyright: (C) 2010 Nicolas Palix, DIKU.
// Copyright: (C) 2010 Julia Lawall, DIKU.
// Copyright: (C) 2010 Gilles Muller, INRIA/LiP6.
>>>>>>> upstream/android-13
// URL: http://coccinelle.lip6.fr/
// Comments:
// Options: --no-includes --include-headers

virtual context
virtual org
virtual report

@r expression@
expression E;
position p;
@@

(
 E@p || ... || E
|
 E@p && ... && E
)

@bad@
expression r.E,e1,e2,fn;
position r.p;
assignment operator op;
@@

(
E@p
&
 <+... \(fn(...)\|e1 op e2\|e1++\|e1--\|++e1\|--e1\) ...+>
)

@depends on context && !bad@
expression r.E;
position r.p;
@@

*E@p

@script:python depends on org && !bad@
p << r.p;
@@

cocci.print_main("duplicated argument to && or ||",p)

@script:python depends on report && !bad@
p << r.p;
@@

coccilib.report.print_report(p[0],"duplicated argument to && or ||")
