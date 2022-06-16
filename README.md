# Multi-Threaded Server and Client System Programming

Can open and close pipes. Sending IP, Port to workers, can no longer print statistics. Workers can get countries and compile statistics, send statistics to the server and I was very close to being able to wait for requests from the server. I have fixed a lot of things so the statistics and the whole process I think works well. <br/> <br/>

The server is multithreaded with pool where they enter queue. We open the threads which can read if the connection is from a worker or client, and when they do not accept requests they add the data to the queue. The thread function can print the statistics it has received and accept the ip, ports and the number of workers. It can also accept queries from the client and print them. There is a function for queries which however does not do anything because I have not dealt with queries. Finally there is a function that starts the links for query, statistics.<br/> <br/>

Client is single-threaded although much of the code is annotated because I could not solve some issues in multi.

3η Άσκηση Προγραμματισμός Συστήματος
ΑΜ: 1115201600278
Στεβής Χαράλαμπος Αντώνιος

Η εργασία αποτελείται από τα εξής:
Φάκελο BASH-> μέσα υπάρχει το shell script και δύο αρχείο με χώρες και ασθένειες
Φάκελος MASTER -> υλικό της δεύτερης άσκησης
    Φάκελος BUILD -> όλα τα .ο αρχεία
    Φάκελος HEADERS -> όλα τα .h αρχεία
    Φάκελος SRC -> όλα τα .c αρχεία
    Φάκελος files -> αποθηκεύονται τα αρχεία των pipes
    Makefile
Φάκελος SERVER-> ότι χρειάζεται για να τρέξει ο σέρβερ
    Φάκελος BUILD -> όλα τα .ο αρχεία
    Φάκελος HEADERS -> όλα τα .h αρχεία
    Φάκελος SRC -> όλα τα .c αρχεία
    Makefile
Φάκελος CLIENT -> ότι χρειαεται ο clint για να τρέξει
    Φάκελος BUILD -> όλα τα .ο αρχεία
    Φάκελος HEADERS -> όλα τα .h αρχεία
    Φάκελος SRC -> όλα τα .c αρχεία
    Φάκελος queries -> Υπάρχει ένα αρχείο κενό που εκεί θα γραφτούν τα queries
    Makefile    
README

Κάθε φάκελος έχει το δικό του makefile άρα πρέπει και στους 3 να κάνουμε make.

Τι μπορεί να κάνει η εργασία:
Έχω βελτιώσει κατά πολύ την δεύτερη εργασία μου και αυτά που μπορεί να κάνει ο MASTER είναι τα εξής:
    Μπορεί να διαβάζει όλες τις χώρες και να τις στέλνει στους workers. Να ανοίγει και να κλείνει pipes. Να στελνει το IP , Port στους workers, δεν μπορεί να εκτυπώνει πλέον τα στατιστικά. Οι workers μπορούν να παιρνουν τις χώρες να φτιάχνουν στατιστικά, να στέλνουν τα στατιστικά στον server και ήμουν πολύ κοντά στο να μπορώ να περιμένω αιτήσεις από τον server. Έχω διορθώσει πολλά πράγματα οπότε τα στατιστικά και η όλη διαδικασία πιστεύω ότι λειτουργεί σωστά.

SERVER:
    Ο σερβερ ειναι multithreaded με pool όπου μπάνουν σε queue. Ανοίγουμε τα threads τα οποία μπορούν να διαβάσουν αν η σύνδεση προέρχεται από worker ή client, και όταν δεν δέχονται αιτήματα προσθέτουν στο queue τα δεδομένα. Η συνάρτηση των threads μπορεί εκτυπώνει τα στατιστικα που έχει δεχτει , αν και το έχω σχολιάσει για να μην γίνει χαμός στο terminal, και να δέχεται τα ip,ports και τον αριθμό των workers. Επίσης μπορεί να δεχτεί τα query από τον client και να τα εκτυπώσει. Υπάρχει συνάρτηση για τα queries η οποία ωστόσο δεν κάνει κάτι γιατί δεν έχω ασχοληθεί με query. Τέλος υπάρχει συνάρτηση η οποία ξεκινάει τις συνδέσεις για query,statistics.

CLIENT:
    Τον έχω κάνει single-threaded αν και μεγάλο μέρος του κώδικα είναι σχολιασμένος γιατί δεν μπορούσα να λύσω κάποια θέματα σε multi. Εννοώ ότι ο κώδικας για mutlithreaded υπάρχει ολόκληρος απλώς κάπου μπαγκάρι και για αυτό θα παρακαλούσα να διαβαστεί.

Τι δεν μπορεί να κάνει η εργασία:
Η εργασία δεν μπορεί να επεξεργαστεί queries. Δεν μπορεί να ανοιξει επικοινωνια  από σερβερ προς workers ώστε να τους περάσει τα queries. Δεν είναι multithreaded ο client (αλλά όπως είπα υπάρχει ο κώδικας). Πολύ πιθανό να μου ξεφεύγουν και άλλα.

Η εκτέλεση του MASTER μερικές φορές μπορεί να αποτύχει, αν συμβεί κάτι τέτοιο απλώς εκτελέστε το ξανά.
