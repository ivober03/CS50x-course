SELECT ROUND(AVG(rating), 2) AS 'avg_ratings'
FROM ratings
JOIN movies ON ratings.movie_id = movies.id
WHERE movies.year = 2012;