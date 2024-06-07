#include "PlayersProfile.h"

PlayersProfile::PlayersProfile() {
	int rc = sqlite3_open("profiles.db", &db);
	if (rc) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		exit(0);
	}

	// Create a table if it doesn't exist
	const char* createTableQuery = "CREATE TABLE IF NOT EXISTS profiles (id INTEGER PRIMARY KEY, data TEXT);";
	rc = sqlite3_exec(db, createTableQuery, 0, 0, 0);
	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		exit(0);
	}
}

PlayersProfile::~PlayersProfile() {
	sqlite3_close(db);
}

void PlayersProfile::CreateProfile()
{
	std::cout << "How many players are you playing with? ";
	std::cin >> numOfPlayers;
	NEWLINE

	float initBalance;
	std::cout << "What should the initial balance be? ";
	std::cin >> initBalance;

	std::cin.ignore();
	NEWLINE

	// Resizes the playersScore and playersName vectors for the number of players and initializes the indexes' values with 0 and name inputs, respectively
	std::pair<std::string, float> pairInitValues;
	pairInitValues.first = "";
	pairInitValues.second = 1. * initBalance;
	players = std::vector<std::pair<std::string, float>>(numOfPlayers, pairInitValues);
	NEWLINE;
	for (int index = 0; index < numOfPlayers; index++) {
		std::cout << "Insert the name of Player #" << index + 1 << ": ";
		std::getline(std::cin, players[index].first);
		NEWLINE
	}
}

void PlayersProfile::SaveProfile()
{
	std::string profileData;
    for (int i = 0; i < numOfPlayers; ++i) {
        profileData += players[i].first + ":" + std::to_string(players[i].second);
        if (i < numOfPlayers - 1) {
            profileData += ",";
        }
    }

    if (newProfile) {
        // Insert the profile data into the database
        std::string insertQuery = "INSERT INTO profiles (data) VALUES ('" + profileData + "');";
        int rc = sqlite3_exec(db, insertQuery.c_str(), 0, 0, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            // Fetch the last inserted row's ID
            profileId = sqlite3_last_insert_rowid(db);
            newProfile = false;

            std::cout << "Profile saved successfully!" << std::endl;
        }
    }
    else {
        std::string updateQuery = "UPDATE profiles SET data = '" + profileData + "' WHERE id = " + std::to_string(profileId) + ";";
        int rc = sqlite3_exec(db, updateQuery.c_str(), 0, 0, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "Profile saved successfully!" << std::endl;
        }
    }
}

void PlayersProfile::SelectProfile() {
    newProfile = false;

    // Fetch the profiles from the database
    const char* selectQuery = "SELECT id, data FROM profiles;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, selectQuery, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error 1: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    int numOfProfiles = 0;

    // Display available profiles
    std::cout << "Available Profiles:" << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* profileData = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        numOfProfiles++;
        std::cout << "ID: " << id << ", Data: " << profileData << std::endl;
    }
    if (numOfProfiles == 0) {
        std::cout << "No profiles to choose from, moving on to Creating Profile..." << TWOENDL;
        CreateProfile();
        return;
    }

    // Prompt the user to select a profile
    std::cout << "Enter the ID of the profile you want to select: ";
    std::cin >> profileId;
    while (profileId <= numOfProfiles && profileId > 0) {
        std::cout << "The ID you entered is not valid. Please choose a players' profile by entering the correct ID on your screen (0 - " << numOfProfiles << "). ";
        std::cin >> profileId;
    }

    // Fetch the selected profile data using a stringstream
    std::stringstream fetchQuery;
    fetchQuery << "SELECT data FROM profiles WHERE id = " << profileId << ";";
    rc = sqlite3_prepare_v2(db, fetchQuery.str().c_str(), -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error 2: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* selectedProfileData = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

        // Clear existing players vector
        players.clear();
        std::pair<std::string, float> playerData;

        // Used to make difference between name and balance and helps updating the players vector with the selected profile data
        int tokenIndex = 0;

        // Tokenize the selectedProfileData using std::strtok
        char* token = strtok(const_cast<char*>(selectedProfileData), ":,");

        while (token != nullptr) {
            tokenIndex++;
            if (tokenIndex % 2 == 1) {
                playerData.first = token;
            }
            else {
                playerData.second = std::stof(token);
                players.push_back(playerData);
            } 

            // Move to the next token
            token = strtok(nullptr, ":,");
        }

        numOfPlayers = players.size();

        std::cout << "Profile selected successfully!" << std::endl;
    }
    else {
        std::cout << "Profile not found." << std::endl;
    }
}
