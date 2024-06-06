set work_mem='4GB';

create materialized view stattotals0 as
-- weekly by hours
select * from (
	select 
		titleid,
		sum(
			upper(int4range(
				extract(epoch from date_trunc('hour', now() - interval'1 week'))::int,
				extract(epoch from date_trunc('hour', now()))::int
				)
				* 
				int4range(utime,case when secs is null then null else utime+secs end))
			-
			lower(int4range(
				extract(epoch from date_trunc('hour', now() - interval'1 week'))::int,
				extract(epoch from date_trunc('hour', now()))::int
				)
				* 
				int4range(utime,case when secs is null then null else utime+secs end))
		)::bigint as secs,
		count(distinct xuid) filter (where
			int4range(
				extract(epoch from date_trunc('hour', now() - interval'1 week'))::int,
				extract(epoch from date_trunc('hour', now()))::int)
			&&
			int4range(utime,case when secs is null then null else utime+secs end)
		)::int as players,
		countryid,
		langid
	from presence
	join gamers using(xuid)
	join countries using(countryid)
	join languages using(langid)
	where 
		utime >= extract(epoch from date_trunc('week', now()-interval '1 week'))::int
		and utime < extract(epoch from date_trunc('hour', now()))
		and countryid is not null
		and langid is not null
	group by cube(1),cube(4,5)
) as foo
where players > 0;
