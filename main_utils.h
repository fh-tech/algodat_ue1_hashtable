//
// Created by vik on 25.03.18.
//

#ifndef HASHTABLE_MAIN_UTILS_H
#define HASHTABLE_MAIN_UTILS_H

HashTable hashTable{ 1'000 };

bool validInput(const std::string& id, const std::string& wkn, const std::string& name, bool verbose);

Share* add();

void delete_share();

Share* search();

void plot();

void plot2();

void save();

void load();

void parse_input(std::string& input);

#include "import_utils.h"

enum Command : int {
    ADD = 1,
    DELETE = 2,
    IMPORT = 3,
    SEARCH = 4,
    PLOT = 5,
    SAVE = 6,
    LOAD = 7,
    QUIT = 8,
    INVALID = -1
};

bool validInput(const std::string& id, const std::string& wkn, const std::string& name, bool verbose)
{
    bool valid = !name.empty() && !id.empty() && !wkn.empty();
    if (verbose && !valid)
        std::cout << "No empty values allowed" << std::endl;
    return valid;
}

Share* add()
{
    Share share{};
    std::string name;
    std::string wkn;
    std::string id; //kürzel
    std::string contin;
    Share* share_p;
    do {
        std::cout << "Please enter the name of the share: ";
        std::cin >> name;

        std::cout << "Please enter the WKN(Wertpapierkennnummer): ";
        std::cin >> wkn;

        std::cout << "Please enter the ID(Kürzel): ";
        std::cin >> id;
    } while (!validInput(id, wkn, name, true));
    std::cout << "A new share with the following details will be created: " << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "WKN: " << wkn << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Continue creating? (y/n): ";
    std::cin >> contin;
    if (contin == "y") {
        Share share(name, id, wkn);
        share_p = hashTable.insert(std::move(share)); //ownership to hashtable with move
    } else {
        std::cout << "Operation aborted.";
        return nullptr;
    }

    if (!share_p) {
        std::cout << "Inserting the share failed." << std::endl;
    } else {
        std::cout << "Successfully added the share." << std::endl;
    }
    return share_p;
}

void delete_share()
{
    std::string contin;
    Share* share;
    share = search();
    if (!share) {
        std::cout << "No share with specified information found.";
    } else {
        std::cout << "Found share. Are you sure you want to delete? (y/n): ";
        std::cin >> contin;
        if (contin == "y") {
            hashTable.remove_by_id(share->id);
            std::cout << "Removed the share." << std::endl;
        } else {
            std::cout << "Operation aborted." << std::endl;
        }
    }
}

Share* search()
{
    std::string contin;
    std::string name;
    std::string id;
    Share* share;
    std::cout << "Press (1) to enter the name of the share (2) to enter id of the share: ";
    std::cin >> contin;
    if (contin == "1") {
        std::cout << "Enter name of share: ";
        std::cin >> name;
        // noch überlegen ob wrapper methode notwendig
        share = hashTable.get_by_name(name);
    } else if (contin == "2") {
        std::cout << "Enter id of share: ";
        std::cin >> id;
        share = hashTable.get_by_id(id);
    } else {
        std::cout << "Not a valid option. Quitting." << std::endl;
        return nullptr;
    }
    return share;
}

void plot()
{
    Share* share;
    share = search();
    if (!share) {
        std::cout << "No share with specified information was found. " << std::endl;
    } else {
        std::array<std::array<char, 30>, 20> plot{};
        std::memset(plot.begin(), ' ', plot.size() * plot[0].size());

        auto [daymin, daymax] = std::minmax_element(std::begin(share->days), std::end(share->days),
            [](Day& d0, Day& d1) { return d0.close < d1.close; });

        std::cout << "<--------------------->" << std::endl;
        float delta = daymax->close - daymin->close;
        float delta_row = delta / plot.size();

        for (size_t i = 0; i < share->days.size(); ++i) {
            Day& d = share->days[i];
            plot[static_cast<size_t>(19.0 - ((d.close - daymin->close) / delta_row))][i] = '#';
        }
        std::cout << share->name << "   :    " << share->id << " (" << share->wkn << ")\n";
        for (auto& row : plot) {
            std::cout << std::string(row.cbegin(), row.cend()) << std::endl;
        }
        std::cout << "<--------------------->" << std::endl;
    }
}

void plot2()
{
    Share* share;
    share = search();
    if (!share) {
        std::cout << "No share with specified information was found. " << std::endl;
    } else {
        float daymin{}, daymax{};
        uint8_t maxStars = 20;
        // find max and min of days
        daymin = daymax = share->days[0].close;
        for (auto& d : share->days) {
            if (d.close < daymin)
                daymin = d.close;
            if (d.close > daymax)
                daymax = d.close;
        }
        std::cout << "\n";
        std::cout << std::setw(7) << std::left << "Name:" << share->name << std::endl;
        std::cout << std::setw(7) << std::left << "Id:" << share->id << std::endl;
        std::cout << std::setw(7) << std::left << "Wkn:" << share->wkn << std::endl;
        std::cout << "<--------------------->" << std::endl;
        int j = 0;
        for (auto& d : share->days) {
            uint8_t starsRel = (d.close / daymax) * maxStars;
            //            std::cout << "stars: " << starsRel << std::endl;
            std::cout << std::setw(2) << j << ":";
            for (int i = 0; i < starsRel; i++) {
                std::cout << "#";
            }
            std::cout << "\n";
            j++;
        }
        std::cout << "<--------------------->" << std::endl;
    }
}

void save()
{
    std::cout << "Please enter an absolute file name: \n>";
    std::string file_name;
    std::cin >> file_name;
    try {
        std::ofstream json_file(file_name);

        json j = hashTable;

        json_file << j << std::flush;

    } catch (...) {
        std::cout << "unable to read json file" << std::endl;
    }
}

void load()
{
    std::cout << "Please enter a absolute file name: \n>";
    std::string file_name;
    std::cin >> file_name;
    try {
        hashTable = HashTable();

        std::ifstream json_file(file_name);
        json j;
        json_file >> j;

        hashTable.m_idTable.resize(j["capacity"]);
        hashTable.m_nameTable.resize(j["capacity"]);
        for (auto ele : j["elements"]) {
            hashTable.insert(ele);
        }

    } catch (...) {
        std::cout << "unable to read json file" << std::endl;
    }
}

void parse_input(std::string& input)
{
    int command;
    try {
        command = std::stoi(input);
        if (command < 1 || command > 8)
            command = -1;
    } catch (std::invalid_argument) {
        command = -1;
    }

    switch (command) {
    case ADD:
        add();
        return;
    case DELETE:
        delete_share();
        return;
    case IMPORT:
        import();
        return;
    case PLOT:
        plot();
        return;
    case SAVE:
        save();
        return;
    case LOAD:
        load();
        return;
    case QUIT:
        std::cout << "Quitting program." << std::endl;
        exit(0);
    case SEARCH:
        Share* share;
        share = search();
        if (!share) {
            std::cout << "Share could not be found." << std::endl;
        } else {
            std::cout << "Share found." << std::endl;
            share->days[0].printDay();
        }
        return;
    case INVALID:
        std::cout << "Wrong input. Try again." << std::endl;
        return;
    default:
        exit(1);
    }
}
//REVIEW the json save part daniel made
//TODO: write tests for everything too (think about some edge cases)
//FIXME: adding share, then saving, then loading, then quit --> double free or corruption (out)

#endif //HASHTABLE_MAIN_UTILS_H
