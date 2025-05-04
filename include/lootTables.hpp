#ifndef LOOTTABLES_HPP
#define LOOTTABLES_HPP

#include <QMap>
#include <QtGlobal>
#include <random>

class LootTables {
private:
    // loots["loottable.json"] to get the list of loots
    static QMap<QString, QList<QString>*>* loots;
    // weights["loottable.json"] to get the list of weights, in the same order as loots
    static QMap<QString, std::discrete_distribution<>>* weights;

    static std::random_device rd;
    static std::mt19937 mtGen;

    LootTables();
    ~LootTables();

    static void addTable(const QString& tableName);
    static void saveDistribution(const QString& tableName, QList<QString>* loots, QList<qreal>* weights);

public:
    static void generateTables();
    static void deleteTables();
    static QString getRandomLoot(const QString& lootTable);
};

// Initialize static variables
inline QMap<QString, std::discrete_distribution<>>* LootTables::weights = nullptr;
inline QMap<QString, QList<QString>*>* LootTables::loots = nullptr;

inline std::random_device LootTables::rd = std::random_device();
inline std::mt19937 LootTables::mtGen = std::mt19937(LootTables::rd());

#endif   // LOOTTABLES_HPP