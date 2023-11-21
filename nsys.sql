.mode column

ATTACH "eig.sqlite" AS eig;


SELECT name FROM eig.sqlite_master;


SELECT * FROM eig.pragma_table_info("cupti_activity_kind_kernel");
SELECT * FROM eig.pragma_table_info("StringIds");

CREATE TABLE summary AS
SELECT
    COUNT() AS calls,
    SUBSTR(value,1,32) AS name,
    SUM(end-start)*1e-6 AS ms
FROM eig.cupti_activity_kind_kernel INNER JOIN eig.StringIds ON (shortname=id)
GROUP BY shortName
ORDER BY ms DESC
;

SELECT
    calls,
    name,
    PRINTF("%8.2f",ms) AS ms
FROM summary
;
