-- Delete empty rows
DELETE FROM public.hydropost_data
WHERE 	water_level IS NULL;

select * from public.hydropost_data
WHERE 	water_level IS NULL;

CREATE FUNCTION public.somefunc() RETURNS integer AS $$
DECLARE
	hydropost_id_last integer := -1;
	date_time_last text := '';
	v_ctid tid;
	v_hydropost_id integer;
	v_hydropost_platform text;
	v_date_time text;
	v_water_level real;
	v_water_anomal real;
	v_water_mean real;
	v_water_temp real;
	v_air_temp real;
	v_ice real;
	v_object_code text;
	v_object_name text;
	rowvar record;
	cur CURSOR FOR 
		WITH t_repeat as (SELECT * FROM 
		(SELECT hd_in.hydropost_id, hd_in.date_time, COUNT(*) as repeat FROM public.hydropost_data hd_in GROUP BY hd_in.hydropost_id, date_time) s1
		WHERE repeat > 1) 
	SELECT hd.ctid, hd.* FROM public.hydropost_data hd, t_repeat
		WHERE 	t_repeat.hydropost_id = hd.hydropost_id
		and		t_repeat.date_time = hd.date_time
		ORDER BY hd.hydropost_id, hd.date_time;
BEGIN
	--OPEN cur;
	FOR rowvar1 IN cur
	LOOP
		--RAISE NOTICE '%', rowvar1.hydropost_id;
		IF hydropost_id_last = rowvar1.hydropost_id
		THEN
			--DELETE 
			IF date_time_last = rowvar1.date_time
			THEN
				RAISE NOTICE 'REPEAT %, %', rowvar1.hydropost_id, rowvar1.date_time;
				DELETE FROM public.hydropost_data WHERE ctid = rowvar1.ctid;
			ELSE
				RAISE NOTICE 'FIRST %, %', rowvar1.hydropost_id, rowvar1.date_time;
			END IF;
		ELSE
			RAISE NOTICE 'FIRST %, %', rowvar1.hydropost_id, rowvar1.date_time;
		END IF;
		hydropost_id_last := rowvar1.hydropost_id;
		date_time_last := rowvar1.date_time;
	END LOOP;
	
	--WHILE True:
		--FETCH cur INTO v_hydropost_id, v_hydropost_platform, v_date_time, v_water_level, v_water_anomal, v_water_mean, v_water_temp, v_air_temp, v_ice, v_object_code, v_object_name;
		--FETCH cur INTO rowvar;
		--RAISE NOTICE '%', v_hydropost_id;
	
	--FETCH cur INTO rowvar;
	
	--RAISE NOTICE 'Hello world!  %', v_water_temp;
	--CLOSE cur;
	RETURN 1;
END;
$$ LANGUAGE plpgsql;

SELECT public.somefunc();

DROP FUNCTION public.somefunc;
