Εργασία 3η:
Lantzos Stergios, AEM: 2789, e-mail: lstergios@uth.gr
Kwnstantinos Zouroufidis, AEM: 1315, e-mail: zouroufi@uth.gr
Alexandros Leventis, AEM: 2702, e-mail: lalexandros@uth.gr

Compile του αρχείου slob.c(usr/src/linux-5.4.86-dev/mm/):
-Βρισκόμαστε μέσα στον Kernel, οπότε οποιαδήποτε προσπάθεια μεταγλώττισης με -gcc θα αποτύχει. Η εντολή που πρέπει να εκτελέσουμε είναι η:
1. sudo make -j2 (μέσα στον "linux-5.4.86-dev", διότι εκεί βρίσκεται το Makefile)
2. sudo make modules_install
3. sudo make install
4. sudo reboot now

Οι 3 τελευταίες εντολές, χρησιμοποιούνται για την εγκατάσταση των νέων modules και του ίδιου του πυρήνα.

-Αρχεία slab.h:
Στα αρχεία α) usr/src/linux-5.4.86-dev/mm/slab.h και β) usr/src/linux-5.4.86-dev/include/linux/slab.h, τα μόνα που προστέθηκαν, ήταν οι δηλώσεις 
των global μεταβλητών: static long int total_free_mem , total_alloc, οι οποίες χρησιμοποιούνται καθαρά για στατιστικά, τα οποία αντλούνται από τα αντίστοιχα syscalls. 
Παρατηρήσαμε,ότι παρόλο που αναμέναμε μία καθυστέρηση λόγω του Best-fit αλγορίθμου που διατρέχει κάθε λίστα ολόκληρη, το boot ήταν υπερβολικά αργό ,κάτι το οποίο οφείλεται λογικά στον κώδικα. 

Παρόλη την καθυστέρηση, ο αλγόριθμος επιτυγχάνει καλύτερη διαχείριση της μνήμης, αφού εντοπίζει το πιο κατάλληλο memory block, για την εκάστοτε αίτηση, και έχει λιγότερο fragmentation.



