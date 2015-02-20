<?

function sqlconnect()
{
	global $DB_host, $DB_db, $DB_user, $DB_pass;
	global $db_link;
	$db_link = mysql_connect($DB_host, $DB_user, $DB_pass)
		or die("Nie mozna polaczyc z $DB_user@$DB_host: " . mysql_error());
	mysql_select_db($DB_db)
		or die('nie mozna wybrac bazy');
}

function sqlexraw($query)
{
	return mysql_query($query);
}

function sqlex($query)
{
	#echo $query;
	$k = sqlexraw($query);
	return vassert($k, "Zapytanie SQL: |$query|".mysql_error());
}

function sql_free($k)
{
	return mysql_free_result($k);
}

function sql_numrows($k)
{
	return mysql_num_rows($k);
}

function sql_numfields($k)
{
	return mysql_num_fields($k);
}

function sql_fetch_array($k, $num)
{
	mysql_data_seek($k, $num);
	return mysql_fetch_array($k,MYSQL_ASSOC);
}

function sql_fetch_row($k, $num)
{
	mysql_data_seek($k, $num);
	return mysql_fetch_row($k);
}

function sqldisconnect($action = 'COMMIT')
{
}


?>
