.mode col


SELECT * FROM sqlite_master;

SELECT * FROM pragma_table_info('ops');
SELECT * FROM pragma_table_info('hip');

SELECT 
count() AS calls,
SUBSTR(Name,1,56) AS kernel,
SUM(DurationNs)*1e-6 AS ms
FROM ops
GROUP BY kernel
ORDER BY ms DESC
;

SELECT 
count() AS calls,
SUBSTR(Name,1,56) AS kernel,
SUM(DurationNs)*1e-6 AS ms
FROM hip
GROUP BY kernel
ORDER BY ms DESC
;

SELECT 
count() AS calls,
SUBSTR(Name,1,56) AS kernel,
SUM(DurationNs)*1e-6 AS ms
FROM copy
GROUP BY kernel
ORDER BY ms DESC
;

PRINTF("");

SELECT 
count() AS calls,
SUBSTR(Name,1,56) AS kernel,
SUM(DurationNs)*1e-6 AS ms
FROM hsa
GROUP BY kernel
ORDER BY ms DESC
;
