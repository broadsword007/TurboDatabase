#include <QCoreApplication>
#include <qhash.h>
#include <qdebug.h>
#include "database.h"
#include "table.h"
#include "record.h"
#include "btreeindex.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QHash<QString, QString> attribute_type_hash ;
    attribute_type_hash["rollno"]="int" ;
    attribute_type_hash["name"]="string" ;

    Table student("Students", attribute_type_hash) ;

    QHash<QString, QString> r1_hash ;
    r1_hash["rollno"]="144326";
    r1_hash["name"]="Syed Ferhan Haider" ;
    Record r1(r1_hash) ;

    QHash<QString, QString> r2_hash ;
    r2_hash["rollno"]="144324";
    r2_hash["name"]="Shayan Shokat" ;
    Record r2(r2_hash) ;

    QHash<QString, QString> r3_hash ;
    r3_hash["rollno"]="144333";
    r3_hash["name"]="Asjad Zaheer" ;
    Record r3(r3_hash) ;


    Database mydb ;
    mydb.addTable(&student) ;

    Index* index=new BTreeIndex(3, "primary", "rollno", &student) ;
    mydb.add_index("Students", index) ;
    mydb.createRecord("Students", &r1) ;
    mydb.createRecord("Students", &r2) ;
    mydb.createRecord("Students", &r3) ;

    //qDebug()<<"Initiating deletion" ;

    //mydb.deleteRecord("Students", "rollno", "144333") ;
    //mydb.deleteRecord("Students", "rollno", "144326") ;
    //mydb.deleteRecord("Students", "rollno", "144324") ;

    QHash<QString, QString> r4_hash ;
    r4_hash["rollno"]="144075";
    r4_hash["name"]="Farzam Azhar" ;
    Record r4(r4_hash) ;

    QHash<QString, QString> r5_hash ;
    r5_hash["rollno"]="144321";
    r5_hash["name"]="Faisal Ali" ;
    Record r5(r5_hash) ;

    QHash<QString, QString> r6_hash ;
    r6_hash["rollno"]="144342";
    r6_hash["name"]="Mr. XYZ" ;
    Record r6(r6_hash) ;

    mydb.createRecord("Students", &r4) ;
    mydb.createRecord("Students", &r5) ;
    mydb.createRecord("Students", &r6) ;

    QList<QString> attr_list;
    attr_list.append("rollno") ;
    attr_list.append("name") ;
    //mydb.updateRecord("Students", "name", "Bitch", "rollno", "144326") ;

    //mydb.deleteRecord("Students", "rollno", "144326") ;

    /*QHash<QString, QString> r7_hash ;
    r7_hash["rollno"]="144000";
    r7_hash["name"]="Mr. ABC" ;
    Record r7(r7_hash) ;

    mydb.createRecord("Students", &r7) ;

    QHash<QString, QString> r8_hash ;
    r8_hash["rollno"]="144325";
    r8_hash["name"]="Mr. KLM" ;
    Record r8(r8_hash) ;

    mydb.createRecord("Students", &r8) ;*/

    mydb.deleteRecord("Students", "rollno", "144324") ;
    mydb.deleteRecord("Students", "rollno", "144075") ;
    mydb.print_table("Students") ;
    return a.exec();
}
