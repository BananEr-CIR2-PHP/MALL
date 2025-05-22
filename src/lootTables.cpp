#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "../include/lootTables.hpp"

#define LOOTTABLES_PATH "../res/loottables/"

// Nothing here (static)
LootTables::LootTables() { }
LootTables::~LootTables() { }

/**
 * Static method.
 * Generate loot tables. Automatically called when trying to access loot tables for the first time.
 * You can call this method to preprocess the generation.
 * /!\ This class is not able to delete its own tables. Please call deleteTables() to avoid memory leaks
 */
void LootTables::generateTables() {
    // TODO: when adding a new loot table, the table should be added here to let the script know the table exists
    loots = new QMap<QString, QList<QString>*>();
    weights = new QMap<QString, std::discrete_distribution<>>();

    addTable("common_mob.json");
    addTable("rare_mob.json");
    addTable("legendary_mob.json");
    addTable("common_weapon.json");
    addTable("legendary_weapon.json");
    addTable("rare_weapon.json");
}

/**
 * Add the table with the given name to loots and weights
 * 
 * @param tableName Name of the table (should look like "foo.json")
 */
void LootTables::addTable(const QString& tableName) {
    QList<QString>* newLoots = new QList<QString>();
    QList<qreal>* newWeights = new QList<qreal>();

    // Open file
    QFile file = QFile(LOOTTABLES_PATH + tableName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file" << (LOOTTABLES_PATH + tableName);
        newLoots->append("Nothing");
        newWeights->append(1);
        saveDistribution(tableName, newLoots, newWeights);
        delete newWeights;
        return;
    }

    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON data.";
        newLoots->append("Nothing");
        newWeights->append(1);
        saveDistribution(tableName, newLoots, newWeights);
        delete newWeights;
        return;
    }

    // Load attributes
    QJsonArray jsonLoots = doc.object()["items"].toArray();
    for (qsizetype i=0; i<jsonLoots.size(); i++) {
        QJsonObject jsonLoot = jsonLoots[i].toObject();
        newLoots->append(jsonLoot["item"].toString());
        newWeights->append(jsonLoot["weight"].toDouble());
    }

    saveDistribution(tableName, newLoots, newWeights);
    delete newWeights;
}

/**
 * Static method.
 * Delete cached tables. Should be called before end of script.
 * Not calling this method may cause memory leaks.
 */
void LootTables::deleteTables() {
    if (loots) {
        qDeleteAll(*loots);
        delete loots;
        loots = nullptr;
    }
    delete weights;
    weights = nullptr;
}

/**
 * Save distribution to the static maps
 * 
 * @param tableName name of the table
 * @param newLoots Loots of the new table
 * @param newWeights weights of the new table
 */
void LootTables::saveDistribution(const QString& tableName, QList<QString>* newLoots, QList<qreal>* newWeights) {
    loots->insert(tableName, newLoots);
    weights->insert(
        tableName,
        std::discrete_distribution(newWeights->begin(), newWeights->end())
    );
}

/**
 * Static method.
 * Get a random loot from the given loot table
 * 
 * @param lootTable Loot table to get the random loot from (should look like "foo.json")
 */
QString LootTables::getRandomLoot(const QString& lootTable) {
    if (! (weights || loots)) {
        generateTables();
    }

    if (lootTable == "") {
        return "Nothing";
    }
    else {
        // Randomness solution comes from:
        // https://en.cppreference.com/w/cpp/numeric/random/discrete_distribution

        int rdm_lootIndex = weights->value(lootTable)(mtGen);
        if (loots->contains(lootTable)) {
            return loots->value(lootTable)->at(rdm_lootIndex);
        }
        else {
            qWarning() << "Loot table " << lootTable << " is unknown LootTable loader.";
            return "";
        }
    }
}