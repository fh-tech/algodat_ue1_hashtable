#include "hashmap.h"

std::string import() {}

int main()
{
    HashTable hashtable(1000);
    std::string name = "Microsoft";
    std::string wkn = "1234";
    std::string id = "MSFT";

    auto share = Share{ "Microsoft", "1234", "MSFT" };

    hashtable.insert(std::move(share));

    Share* s_name = hashtable.get_by_name(name);
    Share* s_wkn = hashtable.get_by_wkn(wkn);

    std::cout << s_name << std::endl;
    std::cout << s_wkn << std::endl;



    return 0;
}
