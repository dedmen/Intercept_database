#include "query.h"
#include <mariadb++/statement.hpp>

using namespace intercept::client;


game_data* createGameDataDBQuery(param_archive* ar) {
    auto x = new GameDataDBQuery();
    if (ar)
        x->serialize(*ar);
    return x;
}


game_value Query::cmd_prepareQuery(game_state&, game_value_parameter right) {

    auto query = new GameDataDBQuery();

    query->queryString = right;

    return query;
}

game_value Query::cmd_prepareQueryAr(game_state&, game_value_parameter right) {
    auto query = new GameDataDBQuery();
    //#TODO verify that right has 2 entries
    query->queryString = right[0];

    for (auto& it : right[1].to_array())
        query->boundValues.emplace_back(it);

    return query;
}
game_value Query::cmd_prepareQueryConfig(game_state&, game_value_parameter right) {
    if (Config::get().getQuery(right).empty()) return {};

    auto query = new GameDataDBQuery();

    query->queryString = right;
    query->isConfigQuery = true;

    return query;
}
game_value Query::cmd_prepareQueryConfigAr(game_state&, game_value_parameter right) {
    if (Config::get().getQuery(right[0]).empty()) return {};

    auto query = new GameDataDBQuery();
    //#TODO verify that right has 2 entries

    query->queryString = right[0];
    query->isConfigQuery = true;

    for (auto& it : right[1].to_array())
        query->boundValues.emplace_back(it);

    return query;
}

game_value Query::cmd_copyQuery(game_state&, game_value_parameter right) {
    auto inp = right.get_as<GameDataDBQuery>();

    auto query = new GameDataDBQuery();

    query->queryString = inp->queryString;
    query->isConfigQuery = inp->isConfigQuery;
    query->boundValues = inp->boundValues;

    return query;
}

game_value Query::cmd_bindValue(game_state&, game_value_parameter left, game_value_parameter right) {
    auto query = left.get_as<GameDataDBQuery>();

    query->boundValues.emplace_back(right);
    return {};
}
game_value Query::cmd_bindValueArray(game_state&, game_value_parameter left, game_value_parameter right) {
    auto query = left.get_as<GameDataDBQuery>();

    for (auto& it : right.to_array())
        query->boundValues.emplace_back(it);

    return {};
}
game_value Query::cmd_bindNamedValue(game_state&, game_value_parameter left, game_value_parameter right) {
    return {};
}
game_value Query::cmd_bindNamedValueArray(game_state&, game_value_parameter left, game_value_parameter right) {
    return {};
}

void Query::initCommands() {
    
    auto dbType = host::register_sqf_type("DBQUERRY"sv, "databaseQuery"sv, "TODO"sv, "databaseQuery"sv, createGameDataDBQuery);
    GameDataDBQuery_typeE = dbType.first;
    GameDataDBQuery_type = dbType.second;


    handle_cmd_prepareQuery = client::host::register_sqf_command("dbPrepareQuery", "TODO", Query::cmd_prepareQuery, dbType.first, game_data_type::STRING);
    handle_cmd_prepareQueryAr = client::host::register_sqf_command("dbPrepareQuery", "TODO", Query::cmd_prepareQueryAr, dbType.first, game_data_type::ARRAY);
    handle_cmd_prepareQueryConfig = client::host::register_sqf_command("dbPrepareQueryConfig", "TODO", Query::cmd_prepareQueryConfig, dbType.first, game_data_type::STRING);
    handle_cmd_prepareQueryConfigAr = client::host::register_sqf_command("dbPrepareQueryConfig", "TODO", Query::cmd_prepareQueryConfigAr, dbType.first, game_data_type::ARRAY);
    handle_cmd_copyQuery = client::host::register_sqf_command("dbCopyQuery", "TODO", Query::cmd_copyQuery, dbType.first, dbType.first);



    //#TODO only accept string,scalar,bool
    handle_cmd_bindValue = client::host::register_sqf_command("dbBindValue", "TODO", Query::cmd_bindValue, game_data_type::NOTHING, dbType.first, game_data_type::ANY);
    handle_cmd_bindValueArray = client::host::register_sqf_command("dbBindValueArray", "TODO", Query::cmd_bindValueArray, game_data_type::NOTHING, dbType.first, game_data_type::ARRAY);

    //handle_cmd_bindNamedValue = client::host::register_sqf_command("db_bindNamedValue", "TODO", Query::cmd_bindNamedValue, game_data_type::NOTHING, dbType.first, game_data_type::ANY);
    //handle_cmd_bindNamedValueArray = client::host::register_sqf_command("db_bindNamedValueArray", "TODO", Query::cmd_bindNamedValueArray, game_data_type::NOTHING, dbType.first, game_data_type::ARRAY);


}
