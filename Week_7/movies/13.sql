SELECT DISTINCT name FROM people
JOIN movies ON stars.movie_id = movies.id
JOIN stars ON stars.person_id = people.id
WHERE movies.title IN(
    SELECT DISTINCT title FROM movies
    JOIN stars ON stars.movie_id = movies.id
    JOIN people ON stars.person_id = people.id
    WHERE people.name = 'Kevin Bacon' AND people.birth = 1958
);
