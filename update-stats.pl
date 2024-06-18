#!/usr/bin/perl -w

use DBI;
use Fcntl qw(:flock);

exit unless (flock(DATA, LOCK_EX|LOCK_NB));		# do not run multiple instances

my $dbh = DBI->connect("dbi:Pg:dbname=global;port=5432") || die;
$dbh->do("set work_mem='2GB'");

my $time = time;

# qw/hour day week month quarter year/;
my @typename = qw/hour/;

for( $t = 0; $t != @typename; $t++ )	{

	update_subj($t);

}

$dbh->do("delete from stattab where players = 0");
$dbh->do("delete from stattab where type=0 and utime < extract(epoch from now()-interval '1 week 1 hour')::int");
$dbh->do("vacuum analyze stattab");

$dbh->disconnect();


sub update_subj	{

	my $t = shift;
	my $type = $typename[$t];
	my $ctype = ($t == 4) ? "3 months" : "1 $type";		# interval doesn't support quarter

	my $maxstat = $dbh->selectrow_array("
		select 
			date_trunc('$type', to_timestamp(max(utime)) + interval '$ctype')
		from stattab where type=$t");

	if( ! defined($maxstat) )	{

		$maxstat = $dbh->selectrow_array("select date_trunc('$type', to_timestamp(min(utime))) from presence");

	}

	my $mytrunc = $dbh->selectrow_array("select date_trunc('$type', to_timestamp($time))");
	
	print "$type $maxstat -> $mytrunc by $ctype\n";

	update_type($t, $maxstat, $mytrunc, $ctype);

}

sub update_type	{

	my ($t, $from, $to, $step) = @_;

	print "   Starting $t from $from to $to step $step\n";

	my $te = $dbh->selectrow_array("select extract(epoch from '$to'::timestamp)::int");

	my $cnt = 0;

	while(1)	{
	
		my $t1 = $dbh->selectrow_array("select extract(epoch from '$from'::timestamp)::int");
		my $tn = $dbh->selectrow_array("select '$from'::timestamp + interval '$step'");
		my $t2 = $dbh->selectrow_array("select extract(epoch from '$tn'::timestamp)::int");

		last if($t1 >= $te);	# do not calc current period

		print "    $t1 ... $t2\n";


		my $req = "insert into stattab
			select 
				titleid,
				sum(
					upper(int4range($t1,$t2) * int4range(utime,case when secs is null then null else utime+secs end)) -
					lower(int4range($t1,$t2) * int4range(utime,case when secs is null then null else utime+secs end))
				)::bigint as secs,
				$t1 as utime,
				count(distinct xuid) filter (
					where int4range($t1,$t2) && int4range(utime,case when secs is null then null else utime+secs end)
				),
				countryid,
				langid,
				$t
			from presence
			join gamers using(xuid)
			join countries using(countryid)
			join languages using(langid)
			where 
				utime >= extract(epoch from date_trunc('week', '$from'::timestamp))::int
				and utime < $t2
				and countryid is not null
				and langid is not null
			group by cube(1),3,cube(5,6),7
		";

		$dbh->do($req);

		$cnt++;

		$from = $tn;

	}

	if($cnt > 0 and $t < 3)	{

		# hour, day, week
		# clean-up
		$dbh->do("delete from stattab where type = $t and players = 0");

		# $dbh->do("refresh materialized view concurrently stattotals$t");
		my $t1 = $dbh->selectrow_array("select extract(epoch from now()-interval '1 week 1 hour')::int");
		my $t2 = $dbh->selectrow_array("select extract(epoch from now())::int");
		my $t3 = $dbh->selectrow_array("select extract(epoch from date_trunc('week',now()-interval '1 week'))::int");

		print "t1=$t1 t2=$t2 t3=$t3\n";

		system("faststat/stater 0 $t1 $t2 '(select * from presence where utime>=$t3 and utime<$t2)'");
		$dbh->begin_work();
		$dbh->do("truncate stattotals1");
		$dbh->do("COPY stattotals1 (titleid,countryid,langid,players,secs) from STDIN (format csv)");
		open(F, "< stattab.csv") || die;
		while(<F>)	{
			$dbh->pg_putcopydata($_);
		}
		close(F);
		$dbh->pg_putcopyend();
		$dbh->commit;
		unlink "stattab.csv";

	}
}



__DATA__
This exists so flock() code above works.
DO NOT REMOVE THIS DATA SECTION.

