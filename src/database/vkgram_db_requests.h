const char select_id_by_username[] =
    "SELECT id FROM vkgram.users WHERE username = '{}' AND password = '{}';";

const char* select_id_by_username_password =
    "SELECT id FROM vkgram.users WHERE username = '{}' AND password = '{}';";

const char* insert_user =
    "INSERT INTO vkgram.users (username, password) VALUES ('{}', '{}');";

const char* select_contacts =
    "WITH contacts_ids AS (\n"
    "    SELECT DISTINCT receiver_id AS id\n"
    "    FROM vkgram.messages\n"
    "    WHERE sender_id = {}\n"
    "\n"
    "    UNION DISTINCT\n"
    "\n"
    "    SELECT DISTINCT sender_id AS id\n"
    "    FROM vkgram.messages\n"
    "    WHERE receiver_id = {}\n"
    ")\n"
    "SELECT u.id, username\n"
    "FROM vkgram.users u\n"
    "JOIN contacts_ids i ON (u.id = i.id);";

const char* get_messages =
    "SELECT mes_id, sender_id, receiver_id, dt, text\n"
    "FROM vkgram.messages\n"
    "WHERE {} IN (sender_id, receiver_id)\n"
    "ORDER BY dt;";

const char* insert_message =
    "INSERT INTO vkgram.messages (sender_id, receiver_id, dt, text)\n"
    "VALUES ({}, {}, NOW(),'{}');";