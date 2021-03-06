Lantzos Stergios -> 2789 -> lstergios@uth.gr
Konstantinos Zouroufidis -> 1315 ->zouroufi@uth.gr
Alexandros Leventis -> 2702 -> lalexandros@uth.gr

Χρόνοι εκτέλεσης profile ανά μέθοδο:

			complex.conf		interactive.conf	noninteractive.conf

min_Goodness		10570 ms		9560 ms			1499 ms		
min_standard_goodness	10753 ms		9322 ms			1499 ms
min_Exp_Burst		10534 ms		9170 ms			1499 ms


Παρατηρήσεις/Σχόλια για τους αλγορίθμους:

1. Όταν μία διεργασία εισέρχεται στην runqueue ή έχει τον επεξεργαστή, η τιμή του Goodness είναι πολύ υψηλή, καθώς η τιμή του waiting_inRQ είναι μηδέν(το οποίο
   βρίσκεται στον παρονομαστή).

2. Με την πάροδο του χρόνου , καθώς το waiting_inRQ παίρνει τιμές, το Goodness της κάθε διεργασίας ομαλοποιείται, εκτός από αυτήν που έχει τον επεξεργαστή. Οπότε, 
   η διεργασία που έχει τον επεξεργαστή, στον επόμενο κύκλο, θα συνεχίσει να εκτελείται μόνο αν δεν υπάρχει άλλη διεργασία(μέθοδος Goodness). 

3. Με τον συνυπολογισμό του waiting_inRQ, ο αλγόριθμος ειναι πιο δίκαιος με τις υπόλοιπες διεργασίες, σε σχέση με το standard_goodness και το Exp_Burst που παρατηρείται 
   starvation σε διεργασίες με μεγάλο Burst_time και περιμένουν πολύ ώρα στην runqueue. Για παράδειγμα στο noninteractive profile(noninteractive.conf)
   με τον αλγόριθμο Exp_Burst, διεργασίες μονοπωλούν τον επεξεργαστή, ενώ με τον αλγόριθμο goodness επιτυγχάνεται πιο δίκαιη αντιμετώπιση των διεργασιών, 
   καθώς λαμβάνεται υπόψιν ο χρόνος αναμονής στην runqueue. 
   
   π.χ. Στο noninteractive profile με την μέθοδο Exp_Burst και -σε μικρότερο βαθμό- στην standard_goodness(βλ. αντίστοιχα outs: noninteractive_Exp_Burst.txt -> γραμμή 650,
   noninteractive_standard_goodness.txt -> γραμμή 1337), παρατηρούμε ότι η διεργασία που δημιουργείται πρώτη (AAAA) , για αρκετή ώρα μονοπωλεί τον cpu αφού δεν υπάρχει
   άλλη διεργασία, με αποτέλεσμα το Burst_time της να είναι αρκετά μεγάλο. Έτσι, οι επόμενες διεργασίες που δημιουργούνται (και έχουν πολύ μικρότερο Exp_Burst) εναλάσ-
   σουν μεταξύ τους τον επεξεργαστή, ενώ η διεργασία ΑΑΑΑ παραμένει στην runqueue για αρκετή ώρα χωρίς να πάρει τον επεξεργαστή(starvation) , με αποτέλεσμα να τελειώνει
   μετά από όλες -σχεδόν- τις άλλες(με το Exp_Burst τελειώνει τελευταία, ενώ με το standard_goodness προτελευταία). 
   Αντίθετα, με τον αλγόριθμο Goodness, βλέπουμε ότι τελειώνει αρκετά πιο νωρίς(βλ. αντίστοιχο out: noninteractive_goodness.txt -> γραμμή 1187), υπάρχει καλύτερη 
   αντιμετώπιση στις διεργασίες που περιμένουν αρκετή ώρα στην runqueue, λόγω της μεταβλητής waiting_inRQ(fairness). 

4. Οι μέθοδοι Exp_Burst και standard_goodness, παρουσιάζουν ομοιότητες μεταξύ τους, αφού χρησιμοποιούνε το προβλεπόμενο μελλοντικό ξέσπασμα για να ορίσουν την επόμενη
   διεργασία που θα πάρει τον επεξεργαστή.



   
HINT για την εκτέλεση: πρέπει να γίνει uncomment η εκάστοτε μέθοδος που θέλουμε να χρησιμοποιήσουμε(αρχείο schedule.c).
