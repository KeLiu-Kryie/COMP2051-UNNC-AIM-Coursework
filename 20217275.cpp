/**
 * @File 20217275.cpp
 * @Brief This is a program used to solve bin packing problem by using Variable Neighbourhood Search algorithm including four neighbourhood actions.
 *
 * @Author Ke Liu(20217275)
 * @Email ssykl3@nottingham.edu.cn
 * @Date 2022/4/24
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

using namespace std;

/**
 * This class is used to store the information of each item in each instance, include the index number, the size and
 * the functions to get all these information.
 */
class Item
{
public:
    Item(int);            // Constructor of class Item;
    void setIndex(int);   // Set the index of an item;
    const int getIndex(); // Get the index of an item;
    const int get_size(); // Get the size of an item;
private:
    int index;
    int size;
};

/**
 * This function is the constructor of the class Item and set the size of the item preliminarily.
 *
 * @param size          int type, the size of the item.
 */
Item::Item(int size)
{
    this->size = size;
}

/**
 * This function is used to set the index of each item and it will be used in the loading instance process only.
 *
 * @param idx           int type, the index of the item.
 */
void Item::setIndex(int index)
{
    this->index = index;
}

/**
 * This function is used to get the index of the item.
 *
 * @return              int type, the index of the item.
 */
const int Item::getIndex()
{
    return index;
}

/**
 * This function is used to get the size of the item
 *
 * @return              int type, the size of the item.
 */
const int Item::get_size()
{
    return size;
}

/**
 * This class is used to store the information of Bin like the residual capacity, packed items and
 * some functions that deals with the bins like encase.
 */
class Bin
{
public:
    Bin(int);                  // Constructor of Bin.
    void set_cap_left(int);    // Calculate the residual capacity of a bin and set it.
    const int get_cap_left();  // Get the residual capacity of the bin.
    void encase(Item);         // Put an item into a bin.
    vector<Item> packed_items; // The packed items in the bin.
private:
    int capacity;
    int cap_left;
};

/**
 * This function is the constructor of Bin and set the capacity and residual capacity preliminarily.
 *
 * @param capacity      int type, the capacity of a bin.
 */
Bin::Bin(int capacity)
{
    this->capacity = capacity;
    cap_left = capacity;
}

/**
 * This function is used to calculate and set the residual capacity of a bin.
 *
 * @param cap_left      int type, the residual capacity of a bin.
 */
void Bin::set_cap_left(int cap_left)
{
    this->cap_left = cap_left;
}

/**
 * This function is used to get the residual capacity of the bin.
 *
 * @return              int type, the residual capacity of the bin.
 */
const int Bin::get_cap_left()
{
    return cap_left;
}

/**
 * This function is used to put an item into a bin and calculate the residual capacity.
 *
 * @param item          Item type, the item that should be put in.
 */
void Bin::encase(Item item)
{
    packed_items.push_back(item);
    set_cap_left(get_cap_left() - item.get_size());
}

/**
 * This class is used to store the information of the problem like the identifier, the capacity, the best solution, all the items,
 * read file function and other relevant functions.
 */
class Problem
{
public:
    Problem(ifstream &inFile);     // The constructor of class Problem.
    const string get_identifier(); // Get the identifier of an instance
    const int get_capacity();      // Get the capacity of an instance.
    const int get_num_items();     // Get the total number of items of an instance.
    const int get_best_solution(); // Get the best solution of the instance.
    void deleteItem(Item);         // Delete an item from the instance.
    vector<Item> items;            // All the items of a instance.
private:
    string identifier;
    int capacity;
    int num_items;
    int best_solution;
};

/**
 * This function is the Constructor of class Problem. This function gets the basic information of an instance like
 * identifier, capacity, the best solution and every item. And this function also sort all these items in a decreasing
 * sequence by using bubble sorting.
 *
 * @param inFile        ifstream reference type, the target file.
 */
Problem::Problem(ifstream &inFile)
{
    string str;
    string substr;
    getline(inFile, str);
    identifier = str.substr(1, str.length() - 1); // Get the problem identifier.
    getline(inFile, str);
    capacity = atoi(str.c_str()); // Get the capacity of bins.
    substr = to_string(capacity);
    str.erase(0, substr.length() + 1);
    num_items = atoi(str.c_str()); // Get the number of items.
    substr = to_string(num_items);
    str.erase(0, substr.length() + 1);
    best_solution = atoi(str.c_str()); // Get the best solution.
    for (int i = 0; i < num_items; i++)
    { // Store the items.
        getline(inFile, str);
        items.push_back(atoi(str.c_str()));
        items[i].setIndex(i);
    }
    /**
     * Bubble sorting in decreasing sequence.
     */
    for (int i = 0; i < items.size(); i++)
    {
        for (int j = 0; j < items.size() - 1; j++)
        {
            if (items[j].get_size() < items[j + 1].get_size())
            {
                Item item = items[j];
                items[j] = items[j + 1];
                items[j + 1] = item;
            }
        }
    }
}

/**
 * This function is used to get the identifier of a problem instance.
 *
 * @return              string type, the identifier of the problem instance.
 */
const string Problem::get_identifier()
{
    return identifier;
}

/**
 * This function is used to get the capacity of a problem instance.
 *
 * @return              int type, the capacity of the problem instance
 */
const int Problem::get_capacity()
{
    return capacity;
}

/**
 * This function is used to get the quantity of the items
 *
 * @return              int type, the total number of the items of the problem instance.
 */
const int Problem::get_num_items()
{
    return num_items;
}

/**
 * This function is used to get the number of bins in the current best known solution.
 *
 * @return              int type, the number of bins in the best known solution.
 */
const int Problem::get_best_solution()
{
    return best_solution;
}

/**
 * This function is used to move the item out from the
 *
 * @param item          Item item. the target item to move out.
 */
void Problem::deleteItem(Item item)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].getIndex() == item.getIndex())
        {
            items.erase(items.begin() + i);
            return;
        }
    }
}

/**
 * This class is used to store the information of the solution like the packed bins, the number of bins, the abs-gap and
 * all the disposal of the bins and items including: relaxed minimum bin slack to get the initial solution. And the variable
 * neighborhood search(VNS) to improve the solution to reduce bins. The VNS can be divided into two parts:Variable neighborhood
 * descent(VND) and Shaking procedure. And a fitness function to evaluate the better solution.
 *
 * VND(4 actions):
 * 1. shift: This action selects a item from the bin that has the largest residual capacity and tries to move the smallest item
 * to the rest of bins by using best fit descent.
 * 2. exchange_largest: This action picks the largest item from the item which has the maximal residual capacity and tries to
 * change it with another smaller item from other bins by using best fit descent.
 * 3: exchange_smallest: This action picks the smallest item from the item which has the maximal residual capacity and tries to
 * change it with another smaller item from other bins by using best fit descents.
 * 4: split: This action randomly select half items from a bin that exceeds the average items per bin to a new bin.
 *
 * Shaking procedure: The shaking procedure is to randomly select two items from two different bins and swap the two items.
 *
 * Fitness function:
 * 1. If the new solution has fewer bins, then the new solution is better.
 * 2. If the new solution has the same bins with the old one, then compare the two bins that have been disposed.
 * If the absolute value of the difference between the residual capacity of the two after-disposed bins is larger than the
 * absolute value of the difference between the residual capacity of the two before-disposed bins, then the new solution is better.
 * 3. If the new solution has more bins, then compare the items in the new bin and the bin with the maximal residual capacity.
 * If there is one item in the new bin has fewer size than any item in the bin with the maximal residual capacity, then the new
 * solution is better.
 */
class Solution
{
public:
    Solution(Problem);                                 // The constructor the class Solution.
    void set_objective(int);                           // Set the objective for each problem instance.
    const string get_identifier();                     // Get the identifier for the problem instance.
    const float get_objective();                       // Get the objective for each problem instance.
    const int get_capacity();                          // Get the capacity for each problem instance.
    const float get_abs_gap();                         // Get the abs_gap for each problem instance.
    void set_bins(vector<Bin>);                        // Set the bins for the problem instance.
    void minimumBinSlack(int, int, int, Bin, Problem); // Relaxed mbs for a single bin.
    void initialize(int, Problem);                     // The process of get initial solution.
    vector<Bin> get_bins();                            // Get the current bins of the instance.
    vector<Bin> shift(vector<Bin>);                    // Heuristic 1.
    vector<Bin> exchange_largest(vector<Bin>);         // heuristic 2.
    vector<Bin> exchange_smallest(vector<Bin>);        // Heuristic 3.
    vector<Bin> split(vector<Bin>);                    // Heuristic 4.
    vector<Bin> shaking(vector<Bin>);                  // Shaking procedure by swapping two items randomly.
    vector<Bin> VNS(int);                              // Variable neighbor search.
    bool evaluation(vector<Bin>, vector<Bin>);         // Fitness function to evaluate the current solution.

private:
    string identifier;
    int capacity;
    float objective;
    float abs_gap;
    int best_known;
    vector<Bin> bins;
};

/**
 * This function is constructor of class Solution. This constructor initializes the identifier, capacity, best known solution
 * and prepare enough bins to pack all items.
 *
 * @param problem       Problem type, a whole instance.
 */
Solution::Solution(Problem problem)
{
    this->identifier = problem.get_identifier();
    this->capacity = problem.get_capacity();
    this->best_known = problem.get_best_solution();
    for (int i = 0; i < problem.items.size(); i++)
    {
        Bin bin(capacity);
        bins.push_back(bin);
    }
}

/**
 * This function is used to pack items for every single bin by using relaxed minimum bin slack.
 *
 * @param crack         int type, the permitted crack of a bin.
 * @param initial       int type, the beginning index of an item.
 * @param chosen        int type, the chosen index of a bin.
 * @param bin           Bin type, a bin to pack items.
 * @param problem       Problem problem, problem instance.
 */
void Solution::minimumBinSlack(int crack, int initial, int chosen, Bin bin, Problem problem)
{
    for (int i = initial; i < problem.items.size(); i++)
    {
        if (problem.items[i].get_size() <= bin.get_cap_left())
        {
            bin.encase(problem.items[i]);
            minimumBinSlack(crack, i + 1, chosen, bin, problem);                       // Recursive to the next.
            bin.set_cap_left(bin.get_cap_left() + bin.packed_items.back().get_size()); // Move out the last fo find a more suitable item.
            bin.packed_items.pop_back();
            if (bins[chosen].get_cap_left() <= crack)
            {
                return;
            }
        }
    }
    if (bin.get_cap_left() < bins[chosen].get_cap_left())
    { // If the bin has fewer residual capacity, then select it.
        bins[chosen] = bin;
    }
}

/**
 * This function is used to initialize the solution by using relaxed minimum bin slack until every items are packed.
 *
 * @param crack         int type, the permitted crack.
 * @param problem       Problem type , the instance.
 */
void Solution::initialize(int crack, Problem problem)
{
    int chosenBinIndex = 0;
    while (problem.items.size() != 0)
    {
        bins[chosenBinIndex].encase(problem.items[0]); // Pack one first.
        Bin bridgeBin = bins[chosenBinIndex];
        problem.deleteItem(problem.items[0]);
        /**
         * Remove all the empty bins.
         */
        if (problem.items.empty())
        {
            bool empty = true;
            do
            {
                int num = bins.size();
                int index = -1;
                for (int i = 0; i < num; i++)
                {
                    if (bins[i].packed_items.empty())
                    {
                        index = i;
                        break;
                    }
                }
                if (index != -1)
                {
                    empty = false;
                    bins.erase(bins.begin() + index);
                }
                else
                {
                    empty = true;
                }
            } while (!empty);
            set_objective(bins.size());
            return;
        }
        minimumBinSlack(crack, 0, chosenBinIndex, bridgeBin, problem); // For every bin.
        for (int i = 0; i < bins[chosenBinIndex].packed_items.size(); i++)
        {
            problem.deleteItem(bins[chosenBinIndex].packed_items[i]); // Remove the packed items.
        }
        chosenBinIndex++;
    }
    /**
     * Remove all the empty bins.
     */
    bool empty = true;
    do
    {
        int num = bins.size();
        int index = -1;
        for (int i = 0; i < num; i++)
        {
            if (bins[i].packed_items.empty())
            {
                index = i;
                break;
            }
        }
        if (index != -1)
        {
            empty = false;
            bins.erase(bins.begin() + index);
        }
        else
        {
            empty = true;
        }
    } while (!empty);
    set_objective(bins.size());
}

/**
 * This function is the variable neighborhood search including 4 neighborhoods to solve bin packing.
 * VND(4 actions):
 * 1. shift: This action selects a item from the bin that has the largest residual capacity and tries to move the smallest item
 * to the rest of bins by using best fit descent.
 * 2. exchange_largest: This action picks the largest item from the item which has the maximal residual capacity and tries to
 * change it with another smaller item from other bins by using best fit descent.
 * 3: exchange_smallest: This action picks the smallest item from the item which has the maximal residual capacity and tries to
 * change it with another smaller item from other bins by using best fit descents.
 * 4: split: This action is to randomly select half items from a bin that exceeds the average items per bin
 * to a new bin.
 *
 * Shaking procedure: The shaking procedure picks randomly two items from different bins and swap them.
 *
 * Fitness function:
 * 1. If the new solution has fewer bins, then the new solution is better.
 * 2. If the new solution has the same bins with the old one, then compare the two bins that have been disposed.
 * If the absolute value of the difference between the residual capacity of the two after-disposed bins is larger than the
 * absolute value of the difference between the residual capacity of the two before-disposed bins, then the new solution is better.
 * 3. If the new solution has more bins, then compare the items in the new bin and the bin with the maximal residual capacity.
 * If there is one item in the new bin has fewer size than any item in the bin with the maximal residual capacity, then the new
 * solution is better.
 *
 * @param max_time      int type, the maximum time of running a single instance.
 * @return              vector<Bin> type, the current bins of the solution.
 */
vector<Bin> Solution::VNS(int max_time)
{
    vector<Bin> answer = get_bins();
    vector<Bin> answer_new;
    vector<Bin> answer_best = answer;
    clock_t startTime, endTime;
    int neighborhood = 2;
    int num;
    /**
     * 4 neighborhoods. If find a better one then back to the first neighborhood, otherwise search in the next neighborhood.
     */
    startTime = clock();
    while (true)
    {
        num = 1;
        while (num <= neighborhood)
        {
            endTime = clock();
            if (((double)(endTime - startTime) / CLOCKS_PER_SEC) >= max_time)
            { // Limit of time.
                return answer_best;
            }
            if (num == 1)
            {
                answer_new = shift(answer); // Heuristic 1.
            }
            else if (num == 2)
            {
                answer_new = exchange_largest(answer); // Heuristic 2.
            }
            else if (num == 3)
            {
                answer_new = exchange_smallest(answer); // Heuristic 3.
            }
            else
            {
                answer_new = split(answer); // Heuristic 4.
            }

            if (evaluation(answer_new, answer))
            { // If better, then return to Heuristic1.
                answer = answer_new;
                num = 1;
                continue;
            }
            else
            {
                num++;
            }
        }
        if (evaluation(answer, answer_best))
        { // If the new is better, then set it as best.
            answer_best = answer;
        }
        answer = shaking(answer); // Shaking procedure
    }
}

/**
 * This function is the fitness function to evaluate the solution.
 * 1. If the new solution has fewer bins, then the new solution is better.
 * 2. If the new solution has the same bins with the old one, then compare the two bins that have been disposed.
 * If the absolute value of the difference between the residual capacity of the two after-disposed bins is larger than the
 * absolute value of the difference between the residual capacity of the two before-disposed bins, then the new solution is better.
 * 3. If the new solution has more bins, then compare the items in the new bin and the bin with the maximal residual capacity.
 * If there is one item in the new bin has fewer size than any item in the bin with the maximal residual capacity, then the new
 * solution is better.
 *
 * @param after         vector<Bin> type, the used bins of the new solution.
 * @param before        vector<Bin> type, the used bins of the old solution.
 * @return              bool type, new one better returns true or old one better returns false.
 */
bool Solution::evaluation(vector<Bin> after, vector<Bin> before)
{
    if (after.size() < before.size())
    { // If new solution has fewer bins.
        return true;
    }
    else if (after.size() == before.size())
    { // If has the same number of bins.
        int diffA;
        int diffB;
        int k;
        bool first = true;
        vector<Bin> a = after;
        vector<Bin> b = before;
        for (int i = 0; i < before.size(); i++)
        { // Find the two bins that swapped items.
            if ((i == before.size() - 1) && (first))
            {
                return false;
            }
            if (a[i].get_cap_left() != b[i].get_cap_left() && first)
            {
                diffA = i;
                first = false;
            }
            else if (a[i].get_cap_left() != b[i].get_cap_left() && !first)
            {
                diffB = i;
                break;
            }
        }
        /**
         * If the absolute value of the difference between the residual capacity of the two after-disposed
         * bins is larger than the absolute value of the difference between the residual capacity of the
         * two before-disposed bins, then the new solution is better.
         */
        k = abs(a[diffA].get_cap_left() - a[diffB].get_cap_left()) - abs(b[diffA].get_cap_left() - b[diffB].get_cap_left());
        if (k >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    { // If new solution has more bins.
        int num;
        int max_left = 0;
        for (int i = 0; i < before.size(); i++)
        {
            if (max_left < before[i].get_cap_left())
            {
                max_left = before[i].get_cap_left();
                num = i;
            }
        }
        // If there is one item in the new bin has fewer size than any item in the bin with the maximal residual capacity.
        for (int i = 0; i < after.back().packed_items.size(); i++)
        {
            for (int k = 0; k < after[num].packed_items.size(); i++)
            {
                if (after.back().packed_items[i].get_size() < after[num].packed_items[k].get_size())
                {
                    return true;
                }
            }
        }
        return false;
    }
}

/**
 * This is the first neighborhood of VNS.
 * This action selects a item from the bin that has the largest residual capacity and tries to move the smallest item
 * to the rest of bins by using best fit descent.
 *
 * @param answer        vector<Bin>type, the used bins of the old solution.
 * @return              vector<Bin>type, the used bins of the new solution.
 */
vector<Bin> Solution::shift(vector<Bin> answer)
{
    vector<Bin> temp = answer;
    int index_max_residual = -1;
    int index_min_residual = -1;
    int max_residual = 0;
    int minimum_cap = get_capacity();
    for (int i = 0; i < temp.size(); i++)
    { // Get the index of the bin with the largest residual capacity.
        if (temp[i].get_cap_left() > max_residual)
        {
            max_residual = temp[i].get_cap_left();
            index_max_residual = i; // Index of max residual.
        }
    }
    for (int i = 0; i < temp.size(); i++)
    { // Get the index of the bin that has the minimum residual capacity if it encases the item removed out.
        if (i == index_max_residual)
        {
            continue;
        }
        if (temp[i].get_cap_left() >= temp[index_max_residual].packed_items.back().get_size())
        {                                                                                                // If it can be packed.
            int rest = temp[i].get_cap_left() - temp[index_max_residual].packed_items.back().get_size(); // Get the size after packed.
            if (rest < minimum_cap)
            { // Get the minimum index and residual capacity.
                index_min_residual = i;
                minimum_cap = rest;
            }
        }
    }

    if ((index_max_residual == -1) || (index_min_residual == -1))
    { // If no where could be packed, then go to the next neighborhood.
        return answer;
    }
    else
    {
        // Move the smallest item from the bin which has max left capacity to the most compact bin that can encase the item.
        Item item(temp[index_max_residual].packed_items.back().get_size());
        item.setIndex(temp[index_max_residual].packed_items.back().getIndex());
        temp[index_max_residual].set_cap_left(temp[index_max_residual].get_cap_left() + item.get_size());
        temp[index_max_residual].packed_items.pop_back();
        temp[index_min_residual].encase(item);
        if (temp[index_max_residual].packed_items.empty())
        {
            temp.erase(temp.begin() + index_max_residual);
        }
        return temp;
    }
}

/**
 * This is the second neighborhood of VNS.
 * This action picks the largest item from the item which has the maximal residual capacity and
 * tries to change it with another smaller item from other bins by using best fit descent.
 *
 * @param answer        vector<Bin>type, the used bins of the old solution.
 * @return              vector<Bin>type, the used bins of the new solution.
 */
vector<Bin> Solution::exchange_largest(vector<Bin> answer)
{
    vector<Bin> temp = answer;
    int index_max_residual = -1;
    int index_largest;
    int max_residual = 0;
    for (int i = 0; i < temp.size(); i++)
    { // Get the index of the bin with the largest residual capacity.
        if (temp[i].get_cap_left() > max_residual)
        {
            max_residual = temp[i].get_cap_left();
            index_max_residual = i;
        }
    }
    max_residual = 0;
    for (int i = 0; i < temp[index_max_residual].packed_items.size(); i++)
    { // Get the largest item in the bin with the largest residual capacity.
        if (temp[index_max_residual].packed_items[i].get_size() > max_residual)
        {
            max_residual = temp[index_max_residual].packed_items[i].get_size();
            index_largest = i;
        }
    }
    int index_change = -1;
    int index_change_item = -1;
    int residual = 0;
    /**
     * To find an item smaller then the item in other bin that could be swapped.
     */
    for (int i = 0; i < temp.size(); i++)
    {
        if (i == index_max_residual)
        {
            continue;
        }
        for (int k = 0; k < temp[i].packed_items.size(); k++)
        {
            if ((0 < (temp[index_max_residual].get_cap_left() + temp[index_max_residual].packed_items[index_largest].get_size() - temp[i].packed_items[k].get_size())) && (temp[index_max_residual].get_cap_left() + temp[index_max_residual].packed_items[index_largest].get_size() - temp[i].packed_items[k].get_size() <= get_capacity()) && ((0 <= temp[i].get_cap_left() - temp[index_max_residual].packed_items[index_largest].get_size() + temp[i].packed_items[k].get_size()) && (temp[i].get_cap_left() - temp[index_max_residual].packed_items[index_largest].get_size() + temp[i].packed_items[k].get_size() < get_capacity())) && (temp[index_max_residual].packed_items[index_largest].get_size() > temp[i].packed_items[k].get_size()))
            {
                int val = temp[index_max_residual].get_cap_left() + temp[index_max_residual].packed_items[index_largest].get_size() - temp[i].packed_items[k].get_size();
                if (val > residual)
                {
                    residual = val;
                    index_change = i;
                    index_change_item = k;
                }
            }
        }
    }
    if (index_change == -1 || index_change_item == -1)
    { // If can't find then go to the next neighbourhood.
        return answer;
    }

    Item item1(temp[index_max_residual].packed_items[index_largest].get_size());
    item1.setIndex(temp[index_max_residual].packed_items[index_largest].getIndex());
    Item item2(temp[index_change].packed_items[index_change_item].get_size());
    item2.setIndex(temp[index_change].packed_items[index_change_item].getIndex());
    // Find them successfully and swap them.
    temp[index_max_residual].encase(item2);
    temp[index_max_residual].packed_items.erase(temp[index_max_residual].packed_items.begin() + index_largest);
    temp[index_change].encase(item1);
    temp[index_change].packed_items.erase(temp[index_change].packed_items.begin() + index_change_item);
    return temp;
}

/**
 * This is the third neighborhood of VNS.
 * This action picks the smallest item from the item which has the maximal residual capacity and tries to
 * change it with another smaller item from other bins by using best fit descents.
 *
 * @param answer        vector<Bin>type, the used bins of the old solution.
 * @return              vector<Bin>type, the used bins of the new solution.
 */
vector<Bin> Solution::exchange_smallest(vector<Bin> answer)
{
    vector<Bin> temp = answer;
    int index_max_residual = -1;
    int index_smallest;
    int max_residual = 0;
    for (int i = 0; i < temp.size(); i++)
    { // Get the index of the bin with the largest residual capacity.
        if (temp[i].get_cap_left() > max_residual)
        {
            max_residual = temp[i].get_cap_left();
            index_max_residual = i;
        }
    }
    max_residual = get_capacity();
    for (int i = 0; i < temp[index_max_residual].packed_items.size(); i++)
    { // Get the smallest item in the bin with the largest residual capacity.
        if (temp[index_max_residual].packed_items[i].get_size() < max_residual)
        {
            max_residual = temp[index_max_residual].packed_items[i].get_size();
            index_smallest = i;
        }
    }
    /**
     * To find an item smaller then the item in other bin that could be swapped.
     */
    int index_change = -1;
    int index_change_item = -1;
    int residual = 0;
    for (int i = 0; i < temp.size(); i++)
    {
        if (i == index_max_residual)
        {
            continue;
        }
        for (int k = 0; k < temp[i].packed_items.size(); k++)
        {
            if ((0 < (temp[index_max_residual].get_cap_left() + temp[index_max_residual].packed_items[index_smallest].get_size() - temp[i].packed_items[k].get_size())) && (temp[index_max_residual].get_cap_left() + temp[index_max_residual].packed_items[index_smallest].get_size() - temp[i].packed_items[k].get_size() <= get_capacity()) && ((0 <= temp[i].get_cap_left() - temp[index_max_residual].packed_items[index_smallest].get_size() + temp[i].packed_items[k].get_size()) && (temp[i].get_cap_left() - temp[index_max_residual].packed_items[index_smallest].get_size() + temp[i].packed_items[k].get_size() < get_capacity())) && (temp[index_max_residual].packed_items[index_smallest].get_size() > temp[i].packed_items[k].get_size()))
            {
                int val = temp[index_max_residual].get_cap_left() + temp[index_max_residual].packed_items[index_smallest].get_size() - temp[i].packed_items[k].get_size();
                if (val > residual)
                {
                    residual = val;
                    index_change = i;
                    index_change_item = k;
                }
            }
        }
    }
    if (index_change == -1 || index_change_item == -1)
    { // If can't find then go to the next neighborhood.
        return answer;
    }
    Item item1(temp[index_max_residual].packed_items[index_smallest].get_size());
    item1.setIndex(temp[index_max_residual].packed_items[index_smallest].getIndex());
    Item item2(temp[index_change].packed_items[index_change_item].get_size());
    item2.setIndex(temp[index_change].packed_items[index_change_item].getIndex());
    // Find them successfully and swap them.
    temp[index_max_residual].encase(item1);
    temp[index_max_residual].packed_items.erase(temp[index_max_residual].packed_items.begin() + index_smallest);
    temp[index_change].encase(item1);
    temp[index_change].packed_items.erase(temp[index_change].packed_items.begin() + index_change_item);
    return temp;
}

/**
 * This is the fourth neighborhood of the VNS.
 * This function is to randomly select half items from a bin that exceeds the average items per bin to a new bin.
 *
 * @param answer        vector<Bin>type, the used bins of the old solution.
 * @return              vector<Bin>type, the used bins of the new solution.
 */
vector<Bin> Solution::split(vector<Bin> answer)
{
    vector<Bin> temp = answer;
    float total_items = 0.0;
    float average_items;
    for (int i = 0; i < temp.size(); i++)
    {
        total_items += (float)temp[i].packed_items.size();
        ;
    }
    average_items = total_items / (float)temp.size(); // To get the average items of per items in all bins.
    vector<int> vec;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i].packed_items.size() > average_items)
        {
            vec.push_back(i);
        }
    }
    srand(time(NULL));
    int random_select = rand() % vec.size();
    int times = temp[vec[random_select]].packed_items.size() / 2;
    Bin bin(get_capacity()); // Move half of items from a random bin to a new bin.
    for (int i = 0; i < times; i++)
    {
        int index_item = rand() % temp[vec[random_select]].packed_items.size();
        Item item(temp[vec[random_select]].packed_items[index_item].get_size());
        temp[vec[random_select]].set_cap_left(temp[vec[random_select]].get_cap_left() + item.get_size());
        bin.encase(item);
        temp[vec[random_select]].packed_items.erase(temp[vec[random_select]].packed_items.begin() + index_item);
    }
    temp.push_back(bin);
    return temp;
}

/**
 * This is the shaking procedure of VNS.
 * This action picks randomly two items from different bins and swap them.
 *
 * @param answer        vector<Bin>type, the used bins of the old solution.
 * @return              vector<Bin>type, the used bins of the new solution.
 */
vector<Bin> Solution::shaking(vector<Bin> answer)
{
    vector<Bin> temp = answer;
    srand(time(NULL));
    int random_select_bin1;
    int random_select_item1;
    int random_select_bin2;
    int random_select_item2;
    int a, b, c, d;
    bool judge;
    clock_t startTime, endTime;
    startTime = clock();
    do
    {
        random_select_bin1 = rand() % temp.size(); // Randomly get the bins and indexes.
        random_select_bin2 = rand() % temp.size();
        random_select_item1 = rand() % temp[random_select_bin1].packed_items.size();
        random_select_item2 = rand() % temp[random_select_bin2].packed_items.size();
        a = temp[random_select_bin1].get_cap_left();
        b = temp[random_select_bin2].get_cap_left();
        c = temp[random_select_bin1].packed_items[random_select_item1].get_size();
        d = temp[random_select_bin2].packed_items[random_select_item2].get_size();
        judge = ((0 <= a + c - d) && (a + c - d <= get_capacity()) && (0 <= b - c + d) && (b - c + d) <= get_capacity() && (a != 0) && (b != 0) && (a != b));
        endTime = clock();
        if ((((double)(endTime - startTime) / CLOCKS_PER_SEC) >= 1) && !judge)
        {
            return answer;
        }
    } while (!judge); // Find two items from different bins then do next.
    Item item1(temp[random_select_bin1].packed_items[random_select_item1].get_size());
    item1.setIndex(temp[random_select_bin1].packed_items[random_select_item1].getIndex());
    Item item2(temp[random_select_bin2].packed_items[random_select_item2].get_size());
    item2.setIndex(temp[random_select_bin2].packed_items[random_select_item2].getIndex());

    temp[random_select_bin1].packed_items.erase(temp[random_select_bin1].packed_items.begin() + random_select_item1);
    temp[random_select_bin1].encase(item2);
    temp[random_select_bin2].packed_items.erase(temp[random_select_bin2].packed_items.begin() + random_select_item2);
    temp[random_select_bin2].encase(item1);
    return temp;
}

/**
 * This function is used to calculate the current number of used bins and the abs_gap.
 *
 * @param objective     int type, the number of the current used bins.
 */
void Solution::set_objective(int objective)
{
    this->objective = objective;
    this->abs_gap = objective - best_known;
}

/**
 * This function is used to get the current number of used bins.
 *
 * @return              int type, the number of the current used bins.
 */
const float Solution::get_objective()
{
    return objective;
}
/**
 * This function is used to get the capacity of the solution.
 *
 * @return              int type, the capacity of the solution.
 */
const int Solution::get_capacity()
{
    return capacity;
}

/**
 * This function is used to get the current bins of the solution.
 *
 * @return              vector<Bin> type, the current bins of the solution.
 */
vector<Bin> Solution::get_bins()
{
    return bins;
}

/**
 * This function is used to get the identifier of the current solution.
 *
 * @return              string type, the identifier of the instance.
 */
const string Solution::get_identifier()
{
    return identifier;
}

/**
 * This function is used to get the abs_gap of the instance.
 *
 * @return              float type, the abs_gap of the instance
 */
const float Solution::get_abs_gap()
{
    return abs_gap;
}

/**
 * This function is used to set the currently used bins of the instance.
 *
 * @param bins         vector<Bin> type, the currently used bins of the instance.
 */
void Solution::set_bins(vector<Bin> bins)
{
    this->bins = bins;
}

/**
 * This function is used to regulate the left residual crack according to the capacity of the bins when using relaxed mbs.
 *
 * @param num           int type, the capacity of the bins in the instance.
 * @return              int type, the allowable slack.
 */
int factor(int num)
{
    int temp;
    if (num == 120)
    {
        temp = 3;
    }
    else if (num == 500)
    {
        temp = 5;
    }
    else
    {
        temp = 11000;
    }
    return temp;
}

/**
 * This is the main function of the program as the start of this program. It should read arguments from the command
 * and make sure use the command in the right way.
 *
 * @param argc      int type, the number of arguments.
 * @param argv      array of pointers type, used to store the pointers that point to the string argument in command.
 * @return          Program ends.
 */
int main(int argc, const char *argv[])
{
    char data_file[30] = "";
    char solution_file[30] = "";
    int max_time;
    for (int i = 1; i < argc; i = i + 2)
    { // Get the arguments.
        if (strcmp(argv[i], "-s") == 0)
        {
            strcpy(data_file, argv[i + 1]);
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            strcpy(solution_file, argv[i + 1]);
        }
        else if (strcmp(argv[i], "-t") == 0)
        {
            max_time = atoi(argv[i + 1]);
        }
    }
    string str;
    ifstream inFile;
    ofstream outFile;

    int problem_num;
    inFile.open(data_file, ios::in);       // Open the source file.
    outFile.open(solution_file, ios::out); // New an output file.
    if (!inFile)
    {
        cout << "Error opening source file" << endl;
        return -1;
    }
    getline(inFile, str);
    problem_num = atoi(str.c_str()); // Get the number of test problems
    outFile << problem_num;
    for (int i = 0; i < problem_num; i++)
    {
        Problem problem(inFile);
        Solution solution(problem);
        solution.initialize(factor(problem.get_num_items()), problem);
        solution.set_bins(solution.VNS(max_time));
        solution.set_objective(solution.get_bins().size()); // Print out.
        outFile << endl
                << solution.get_identifier() << endl
                << "obj=    " << solution.get_objective() << "    " << solution.get_abs_gap();
        for (int i = 0; i < solution.get_bins().size(); i++)
        {
            outFile << endl;
            for (int k = 0; k < solution.get_bins()[i].packed_items.size(); k++)
            {
                outFile << solution.get_bins()[i].packed_items[k].getIndex();
                if (k != solution.get_bins()[i].packed_items.size() - 1)
                {
                    outFile << " ";
                }
            }
        }
    }
    inFile.close();  // Close file stream.
    outFile.close(); // Close file stream.
    return 0;
}