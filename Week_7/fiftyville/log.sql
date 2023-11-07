-- This query is to find the report of the crime scene:
SELECT description
    FROM crime_scene_reports
  WHERE month = 7
    AND day = 28
    AND street = 'Humphrey Street';
-- INFO: 10:15am // three witnesses // bakery // Littering 16:36

-- This query is to find the transcription of the interviews of the three witnesses:
SELECT transcript
    FROM interviews
  WHERE month = 7
    AND day = 28
    AND year = 2021;
/* INFO:
        Interview 1: ATM on Leggett Street and saw the thief there withdrawing some money.
        Interview 2: the thief called someone who talked to them for less than a minute.
                     the thief said that they were planning to take the earliest flight out of Fiftyville tomorrow.
                     The thief then asked the person on the other end of the phone to purchase the flight ticket.
        Interview 3: within ten minutes of the theft the thief got into a car in the bakery parking lot and drive away.
*/

-- This query is to find information about the flight mentioned in Interview 2:
SELECT *
    FROM flights
  WHERE day = 29
    AND month = 7
  ORDER BY hour
  LIMIT 1;
-- INFO: Id = 36 // destination_airport = 4

-- This query is to find where did the thief escaped:
SELECT city
    FROM airports
  WHERE id = 4;
-- INFO: City: New York

-- This query is to find the license plates of the people that left the bakery around 10:15am:
SELECT name, bakery_security_logs.hour, bakery_security_logs.minute, bakery_security_logs.license_plate
    FROM people
  JOIN bakery_security_logs
      ON people.license_plate = bakery_security_logs.license_plate
  WHERE bakery_security_logs.year = 2021
    AND bakery_security_logs.month = 7
    AND bakery_security_logs.day = 28
    AND bakery_security_logs.activity = 'exit'
    AND bakery_security_logs.hour = 10
    AND bakery_security_logs.minute >= 15
    AND bakery_security_logs.minute <= 25
  ORDER BY bakery_security_logs.minute;
/* INFO: Principal suspects:
+---------+------+--------+---------------+
|  name   | hour | minute | license_plate |
+---------+------+--------+---------------+
| Vanessa | 10   | 16     | 5P2BI95       |
| Bruce   | 10   | 18     | 94KL13X       |
| Barry   | 10   | 18     | 6P58WS2       |
| Luca    | 10   | 19     | 4328GD8       |
| Sofia   | 10   | 20     | G412CB7       |
| Iman    | 10   | 21     | L93JTIZ       |
| Diana   | 10   | 23     | 322W7JE       |
| Kelsey  | 10   | 23     | 0NTHK55       |
+---------+------+--------+---------------+
*/

-- This query is to find the name of the people who were in the flight to New York and also left the bakery at around 10:15:
SELECT DISTINCT name
    FROM people
  JOIN bakery_security_logs
      ON people.license_plate = bakery_security_logs.license_plate
  JOIN passengers
      ON people.passport_number = passengers.passport_number
  WHERE people.license_plate IN (SELECT license_plate
                                        FROM bakery_security_logs
                                        WHERE year = 2021
                                          AND month = 7
                                          AND day = 28
                                          AND activity = 'exit'
                                          AND hour = 10
                                          AND minute >= 15
                                          AND minute <= 25)
      AND passengers.passport_number IN (SELECT passport_number
                                          FROM passengers
                                          WHERE flight_id = 36);
/* INFO: Principal suspects:
/+--------+
|  name  |
+--------+
| Sofia  |
| Luca   |
| Kelsey |
| Bruce  |
+--------+
*/

-- This query is to find which of the principal suspects made the call that interview 2 mentions:
SELECT people.name, phone_calls.receiver, phone_calls.duration
    FROM people
  JOIN phone_calls
    ON people.phone_number = phone_calls.caller
  WHERE people.name in ('Sofia', 'Luca', 'Kelsey', 'Bruce')
    AND year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60;
-- INFO: The list of suspects remain the same because all of them made a call with the mentioned characteristics

-- This query is to find wich suspects used the ATM that the first interview talks about:
SELECT name
    FROM people
  JOIN bank_accounts
    ON people.id = bank_accounts.person_id
  JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
  WHERE people.name in ('Sofia', 'Luca', 'Kelsey', 'Bruce')
    AND atm_transactions.atm_location = 'Leggett Street'
    AND atm_transactions.year = 2021
    AND atm_transactions.month = 7
    AND atm_transactions.day = 28;
/* INFO: Principal suspects:
/+--------+
|  name  |
+--------+
| Luca   |
| Bruce  |
+--------+
*/

-- This query is to find the number of the accomplice knowing that he/she was on the flight to NY and was called by Luca or Bruce:
SELECT receiver
    FROM phone_calls
  JOIN people
    ON phone_calls.caller = people.phone_number
  JOIN passengers
    ON passengers.passport_number = people.passport_number
  WHERE passengers.passport_number IN (SELECT passport_number
                                        FROM passengers
                                        WHERE flight_id = 36)
    AND phone_calls.year = 2021
    AND phone_calls.month = 7
    AND phone_calls.day = 28
    AND phone_calls.duration < 60
    AND caller IN (SELECT phone_number
                    FROM people
                    WHERE name = 'Luca' OR name = 'Bruce');
-- INFO: Receiver = (375) 555-8161

-- This query is to find the name of the accomplice:
SELECT DISTINCT name
  FROM people
  JOIN phone_calls
    ON people.phone_number = phone_calls.receiver
 WHERE phone_calls.receiver = '(375) 555-8161';
-- INFO: Robin

-- This query is to find the name of the thief:
SELECT DISTINCT name
  FROM people
  JOIN phone_calls
    ON people.phone_number = phone_calls.caller
 WHERE phone_calls.receiver = '(375) 555-8161'
    AND name IN ('Luca', 'Bruce');
-- INFO: Bruce