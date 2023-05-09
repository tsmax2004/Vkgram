CREATE TABLE vkgram.users (
    user_id     SERIAL PRIMARY KEY,
    username    TEXT,
    password    TEXT
);

CREATE TABLE vkgram.messages (
    mes_id          BIGSERIAL PRIMARY KEY,
    sender_id       INT,
    receiver_id     INT,
    dt              TIMESTAMP,
    text            TEXT
);