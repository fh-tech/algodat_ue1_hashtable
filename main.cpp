#include <iostream>
#include <array>
#include <vector>
#include "Hashtable.h"


std::string import() {

}


int main() {
    Hashtable hashtable = Hashtable(1000);
    std::string name = "Microsoft";
    std::string wkn = "1234";
    std::string id = "MSF";
    Share *share = new Share(name,wkn,id);


    hashtable.insert(share);

    Bucket * b_name = hashtable.search_byName(name);
    Bucket * b_wkn = hashtable.search_byWKN(wkn);

    std::cout << b_name->data << std::endl;     //FIXME: wtf i return a pointer to bucket but its contents are fucked
    std::cout << b_wkn->data << std::endl;      //FIXME: wtf i return a pointer to bucket but its contents are fucked
    std::cout << share << std::endl;

    hashtable.printAll<0>();
    hashtable.removeShare(name);
    hashtable.printAll<0>();

    return 0;
}