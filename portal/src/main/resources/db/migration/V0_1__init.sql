-- new schema for storing users

CREATE TABLE users (

  id SERIAL primary key,

  name VARCHAR(255) NOT NULL,

  created TIMESTAMP NOT NULL DEFAULT NOW(),

  last_modified TIMESTAMP NOT NULL DEFAULT NOW(),

  email VARCHAR(255) NOT NULL,

  image_url VARCHAR(255) DEFAULT NULL,

  provider VARCHAR(255) DEFAULT NULL,

  provider_id VARCHAR(255) DEFAULT NULL,

  active BOOLEAN NOT NULL DEFAULT FALSE,

  password TEXT,

  family_name VARCHAR(255) DEFAULT NULL,

  given_name VARCHAR(255) DEFAULT NULL,

  phone VARCHAR(255) DEFAULT NULL,

  timezone VARCHAR(255) DEFAULT NULL,

  preferred_language VARCHAR(255) DEFAULT NULL
);