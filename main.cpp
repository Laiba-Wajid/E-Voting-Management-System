#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;

// ============================================================
//                    BASE CLASS: PERSON
// ============================================================

class Person
{
protected:
    string name;
    string cnic;

public:
    Person() {}

    Person(string n, string c)
    {
        name = n;
        cnic = c;
    }

    virtual void display() const
    {
        cout << "Name : " << name << endl;
        cout << "CNIC : " << cnic << endl;
    }

    string getName() const
    {
        return name;
    }

    string getCNIC() const
    {
        return cnic;
    }

    virtual ~Person() {}
};

// ============================================================
//                    VOTER CLASS
// ============================================================

class Voter : public Person
{
private:
    int age;
    bool hasVoted;

public:
    Voter() : Person()
    {
        age = 0;
        hasVoted = false;
    }

    Voter(string n, string c, int a)
        : Person(n, c)
    {
        age = a;
        hasVoted = false;
    }

    int getAge() const
    {
        return age;
    }

    bool getVotingStatus() const
    {
        return hasVoted;
    }

    void castVote()
    {
        hasVoted = true;
    }

    void resetVote()
    {
        hasVoted = false;
    }

    void display() const override
    {
        cout << left
             << setw(20) << name
             << setw(18) << cnic
             << setw(8) << age
             << setw(15) << (hasVoted ? "Voted" : "Not Voted")
             << endl;
    }

    void saveToFile(ofstream &file) const
    {
        file << name << "|"
             << cnic << "|"
             << age << "|"
             << hasVoted << endl;
    }
};

// ============================================================
//                  CANDIDATE CLASS
// ============================================================

class Candidate : public Person
{
private:
    string party;
    int votes;

public:
    Candidate() : Person()
    {
        party = "";
        votes = 0;
    }

    Candidate(string n, string c, string p)
        : Person(n, c)
    {
        party = p;
        votes = 0;
    }

    string getParty() const
    {
        return party;
    }

    int getVotes() const
    {
        return votes;
    }

    void addVote()
    {
        votes++;
    }

    void display() const override
    {
        cout << left
             << setw(5) << cnic
             << setw(20) << name
             << setw(20) << party
             << setw(10) << votes
             << endl;
    }

    void saveToFile(ofstream &file) const
    {
        file << name << "|"
             << cnic << "|"
             << party << "|"
             << votes << endl;
    }
};

// ============================================================
//                  ELECTION SYSTEM CLASS
// ============================================================

class ElectionSystem
{
private:
    vector<Voter> voters;
    vector<Candidate> candidates;

    const string voterFile = "voters.txt";
    const string candidateFile = "candidates.txt";

public:

    // --------------------------------------------------------
    // Constructor
    // --------------------------------------------------------

    ElectionSystem()
    {
        loadVoters();
        loadCandidates();

        // Add default candidates if file is empty
        if (candidates.empty())
        {
            candidates.push_back(
                Candidate("Ali Khan", "C001", "Unity Party"));

            candidates.push_back(
                Candidate("Ahmed Raza", "C002", "People Party"));

            candidates.push_back(
                Candidate("Sara Malik", "C003", "Progress Party"));

            saveCandidates();
        }
    }

    // --------------------------------------------------------
    // Input helper
    // --------------------------------------------------------

    int getInteger(string message)
    {
        int value;

        while (true)
        {
            cout << message;

            if (cin >> value)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }

            cout << "Invalid input! Please enter a number.\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // --------------------------------------------------------
    // CNIC validation
    // --------------------------------------------------------

    bool validCNIC(string cnic)
    {
        if (cnic.length() != 13)
            return false;

        for (char ch : cnic)
        {
            if (!isdigit(ch))
                return false;
        }

        return true;
    }

    // --------------------------------------------------------
    // Find voter
    // --------------------------------------------------------

    int findVoter(string cnic)
    {
        for (int i = 0; i < voters.size(); i++)
        {
            if (voters[i].getCNIC() == cnic)
                return i;
        }

        return -1;
    }

    // --------------------------------------------------------
    // Find candidate
    // --------------------------------------------------------

    int findCandidate(string cnic)
    {
        for (int i = 0; i < candidates.size(); i++)
        {
            if (candidates[i].getCNIC() == cnic)
                return i;
        }

        return -1;
    }

    // --------------------------------------------------------
    // Register voter
    // --------------------------------------------------------

    void registerVoter()
    {
        string name;
        string cnic;
        int age;

        cout << "\n========== VOTER REGISTRATION ==========\n";

        cout << "Enter full name: ";
        getline(cin, name);

        cout << "Enter 13-digit CNIC: ";
        getline(cin, cnic);

        if (!validCNIC(cnic))
        {
            cout << "Invalid CNIC! CNIC must contain exactly 13 digits.\n";
            return;
        }

        if (findVoter(cnic) != -1)
        {
            cout << "A voter with this CNIC already exists.\n";
            return;
        }

        age = getInteger("Enter age: ");

        if (age < 18)
        {
            cout << "Voter must be 18 or above.\n";
            return;
        }

        voters.push_back(Voter(name, cnic, age));

        saveVoters();

        cout << "\nVoter registered successfully!\n";
    }

    // --------------------------------------------------------
    // Display voters
    // --------------------------------------------------------

    void displayVoters()
    {
        cout << "\n================ REGISTERED VOTERS ================\n";

        if (voters.empty())
        {
            cout << "No voters registered.\n";
            return;
        }

        cout << left
             << setw(20) << "Name"
             << setw(18) << "CNIC"
             << setw(8) << "Age"
             << setw(15) << "Status"
             << endl;

        cout << string(61, '-') << endl;

        for (const auto &v : voters)
        {
            v.display();
        }
    }

    // --------------------------------------------------------
    // Add candidate
    // --------------------------------------------------------

    void addCandidate()
    {
        string name;
        string cnic;
        string party;

        cout << "\n========== ADD CANDIDATE ==========\n";

        cout << "Enter candidate name: ";
        getline(cin, name);

        cout << "Enter candidate ID: ";
        getline(cin, cnic);

        if (findCandidate(cnic) != -1)
        {
            cout << "Candidate ID already exists.\n";
            return;
        }

        cout << "Enter political party: ";
        getline(cin, party);

        candidates.push_back(
            Candidate(name, cnic, party));

        saveCandidates();

        cout << "\nCandidate added successfully!\n";
    }

    // --------------------------------------------------------
    // Display candidates
    // --------------------------------------------------------

    void displayCandidates()
    {
        cout << "\n================ CANDIDATES ================\n";

        if (candidates.empty())
        {
            cout << "No candidates available.\n";
            return;
        }

        cout << left
             << setw(5) << "ID"
             << setw(20) << "Name"
             << setw(20) << "Party"
             << setw(10) << "Votes"
             << endl;

        cout << string(55, '-') << endl;

        for (const auto &c : candidates)
        {
            c.display();
        }
    }

    // --------------------------------------------------------
    // Cast vote
    // --------------------------------------------------------

    void castVote()
    {
        string cnic;

        cout << "\n========== CAST VOTE ==========\n";

        cout << "Enter your 13-digit CNIC: ";
        getline(cin, cnic);

        int voterIndex = findVoter(cnic);

        if (voterIndex == -1)
        {
            cout << "Voter not found! Please register first.\n";
            return;
        }

        if (voters[voterIndex].getVotingStatus())
        {
            cout << "You have already cast your vote.\n";
            return;
        }

        displayCandidates();

        string candidateID;

        cout << "\nEnter Candidate ID: ";
        getline(cin, candidateID);

        int candidateIndex = findCandidate(candidateID);

        if (candidateIndex == -1)
        {
            cout << "Invalid candidate ID.\n";
            return;
        }

        candidates[candidateIndex].addVote();

        voters[voterIndex].castVote();

        saveVoters();
        saveCandidates();

        cout << "\n========================================\n";
        cout << "       VOTE CAST SUCCESSFULLY!\n";
        cout << "========================================\n";
    }

    // --------------------------------------------------------
    // Search voter
    // --------------------------------------------------------

    void searchVoter()
    {
        string cnic;

        cout << "\nEnter voter CNIC: ";
        getline(cin, cnic);

        int index = findVoter(cnic);

        if (index == -1)
        {
            cout << "Voter not found.\n";
            return;
        }

        cout << "\nVoter Found:\n";
        cout << "Name: " << voters[index].getName() << endl;
        cout << "CNIC: " << voters[index].getCNIC() << endl;
        cout << "Age : " << voters[index].getAge() << endl;

        cout << "Voting Status: "
             << (voters[index].getVotingStatus()
                     ? "Already Voted"
                     : "Not Voted")
             << endl;
    }

    // --------------------------------------------------------
    // Election results
    // --------------------------------------------------------

    void displayResults()
    {
        cout << "\n================ ELECTION RESULTS ================\n";

        if (candidates.empty())
        {
            cout << "No candidates available.\n";
            return;
        }

        int totalVotes = 0;

        for (const auto &c : candidates)
        {
            totalVotes += c.getVotes();
        }

        cout << left
             << setw(20) << "Candidate"
             << setw(20) << "Party"
             << setw(10) << "Votes"
             << setw(12) << "Percentage"
             << endl;

        cout << string(62, '-') << endl;

        for (const auto &c : candidates)
        {
            double percentage = 0;

            if (totalVotes > 0)
            {
                percentage =
                    (double)c.getVotes() / totalVotes * 100;
            }

            cout << left
                 << setw(20) << c.getName()
                 << setw(20) << c.getParty()
                 << setw(10) << c.getVotes()
                 << fixed << setprecision(2)
                 << percentage << "%"
                 << endl;
        }

        cout << "\nTotal Votes: " << totalVotes << endl;

        // Find winner
        int winnerIndex = 0;

        for (int i = 1; i < candidates.size(); i++)
        {
            if (candidates[i].getVotes() >
                candidates[winnerIndex].getVotes())
            {
                winnerIndex = i;
            }
        }

        if (totalVotes > 0)
        {
            cout << "\nWinner: "
                 << candidates[winnerIndex].getName()
                 << " (" << candidates[winnerIndex].getParty()
                 << ")" << endl;
        }
        else
        {
            cout << "\nNo votes have been cast yet.\n";
        }
    }

    // --------------------------------------------------------
    // Save voters
    // --------------------------------------------------------

    void saveVoters()
    {
        ofstream file(voterFile);

        if (!file)
        {
            cout << "Error opening voter file.\n";
            return;
        }

        for (const auto &v : voters)
        {
            v.saveToFile(file);
        }

        file.close();
    }

    // --------------------------------------------------------
    // Load voters
    // --------------------------------------------------------

    void loadVoters()
    {
        ifstream file(voterFile);

        if (!file)
            return;

        string name, cnic, ageString, votedString;

        while (getline(file, name, '|') &&
               getline(file, cnic, '|') &&
               getline(file, ageString, '|') &&
               getline(file, votedString))
        {
            try
            {
                int age = stoi(ageString);

                Voter v(name, cnic, age);

                if (votedString == "1")
                {
                    v.castVote();
                }

                voters.push_back(v);
            }
            catch (...)
            {
                // Ignore invalid file records
            }
        }

        file.close();
    }

    // --------------------------------------------------------
    // Save candidates
    // --------------------------------------------------------

    void saveCandidates()
    {
        ofstream file(candidateFile);

        if (!file)
        {
            cout << "Error opening candidate file.\n";
            return;
        }

        for (const auto &c : candidates)
        {
            c.saveToFile(file);
        }

        file.close();
    }

    // --------------------------------------------------------
    // Load candidates
    // --------------------------------------------------------

    void loadCandidates()
    {
        ifstream file(candidateFile);

        if (!file)
            return;

        string name, cnic, party, votesString;

        while (getline(file, name, '|') &&
               getline(file, cnic, '|') &&
               getline(file, party, '|') &&
               getline(file, votesString))
        {
            try
            {
                int votes = stoi(votesString);

                Candidate c(name, cnic, party);

                for (int i = 0; i < votes; i++)
                {
                    c.addVote();
                }

                candidates.push_back(c);
            }
            catch (...)
            {
                // Ignore invalid records
            }
        }

        file.close();
    }

    // --------------------------------------------------------
    // Main menu
    // --------------------------------------------------------

    void menu()
    {
        int choice;

        do
        {
            cout << "\n\n";
            cout << "====================================================\n";
            cout << "             E-VOTING MANAGEMENT SYSTEM             \n";
            cout << "====================================================\n";
            cout << "              C++ OOP PROJECT                       \n";
            cout << "====================================================\n";

            cout << "\n1. Register Voter";
            cout << "\n2. Display All Voters";
            cout << "\n3. Add Candidate";
            cout << "\n4. Display Candidates";
            cout << "\n5. Cast Vote";
            cout << "\n6. Search Voter";
            cout << "\n7. View Election Results";
            cout << "\n8. Exit";

            cout << "\n\nEnter your choice: ";

            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 1:
                registerVoter();
                break;

            case 2:
                displayVoters();
                break;

            case 3:
                addCandidate();
                break;

            case 4:
                displayCandidates();
                break;

            case 5:
                castVote();
                break;

            case 6:
                searchVoter();
                break;

            case 7:
                displayResults();
                break;

            case 8:
                saveVoters();
                saveCandidates();

                cout << "\nThank you for using E-Voting System!\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
            }

        } while (choice != 8);
    }
};

// ============================================================
//                        MAIN FUNCTION
// ============================================================

int main()
{
    ElectionSystem system;

    system.menu();

    return 0;
}
