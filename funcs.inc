<?


# --------------------------- assert

function vassert($val, $reason = '------')
{
	if (!$val)
		die("<b>Błąd: $reason</b>");
	return $val;
}

function typassert($tab, $type)
{
	vassert($tab['typassert'] == $type, "expected '$type' type!");
}

# --------------------------- SQL
function sqlquote($str)
{
	$str=trim($str);
	if (strlen($str)>0)
		$str = "'".addslashes($str)."'";
		#$str = "'".preg_replace('/\'/', '\\\'', $str)."'";
	else
		$str="NULL";
	return $str;
}



#function sqlconnect()
#function sqlex($query)
#function sql_numrows($k)
#function sql_fetch_array($k, $num)
#function sql_fetch_row($k, $num)
#function sqldisconnect($action = 'COMMIT')
include 'funcs-db.inc';

function sql_rows($tabelka, $distinct = false)
{
	if ($distinct)
	{
		$e = sqlex("SELECT DISTINCT * FROM $tabelka");
		return sql_numrows($e);
	}
	$e = sqlex("SELECT count(*) as c from $tabelka");
	$arr = sql_fetch_array($e, 0);
	return $arr['c'];
}

function sqlexrow($query, $field = null)
{
	$e = sqlex($query);
	if (sql_numrows($e) > 0)
	{
		$arr = sql_fetch_array($e, 0);
		return empty($field) ? $arr : $arr[$field];
	}
	else
		return null;
}

function sql_insert_quoted($table, $arr)
{
	$fields =''; $values='';
	$przecinek = '';
	foreach ($arr as $field => $value)
	{
		$fields .= $przecinek . $field;
		$values .= $przecinek . sqlquote($value);
		$przecinek = ', ';
	}
	$query = "INSERT INTO $table ($fields) VALUES ($values)";
	return sqlexraw($query);
}


function sql_update_quoted($table, $arr, $where = 'true')
{
	$set=''; $przecinek = '';
	foreach ($arr as $field => $value)
	{
		$set .= $przecinek . $field . '=' . sqlquote($value);
		$przecinek = ', ';
	}
	$query = "UPDATE $table SET $set WHERE $where";
	#echo "|$query|";
	return sqlexraw($query);
}

function sqlseqval($sequence)
{
	return sqlexrow("SELECT currval('${sequence}_id_seq') as cv", 'cv');
}


/* zwraca tablicę 1 LUB 2 - wymiarową */
function sql2tab($query)
{
	$tab = array();
	$e = sqlex($query);
	$rows = sql_numrows($e);
	for ($r = 0; $r<$rows; $r++)
	{
		$arr = sql_fetch_row($e, $r);
		if (sizeof($arr) > 1)
			for ($c = 0; $c<sizeof($arr); $c++)
				$tab[$r][$c] = $arr[$c];
		else
			$tab[$r] = $arr[0];
	}
	return $tab;
}

function sql2assoc($query)
{
	$tab = array();
	$e = sqlex($query);
	$rows = sql_numrows($e);
	for ($r = 0; $r<$rows; $r++)
	{
		$row = sql_fetch_array($e, $r);
		$tab[] = $row;
	}
	return $tab;
}

/* zwraca tablice id => wartosc */
function sqlpairs($query)
{
	$r = array();
	$tab = sql2tab($query);
	foreach ($tab as $t)
		$r[$t[0]] = $t[1];
	return $r;
}

function fixurl($url)
{
	if (empty($url))
		return $url;
	if (ereg('^[a-zA-Z]+://$',$url))
		return '';
	if (!ereg('^[a-zA-Z]+://',$url))
		$url = "http://$url";
	return $url;
}


function int_par($var, $default=0)
{
	if (!empty($_GET[$var]))
		return (int)$_GET[$var];
	elseif (!empty($_POST[$var]))
		return (int)$_POST[$var];
	else
		return $default;
}

function str_par($var, $default='')
{
	if (!empty($_GET[$var]))
		return $_GET[$var];
	elseif (!empty($_POST[$var]))
		return $_POST[$var];
	else
		return $default;
}

function cent_par($var, $default=0)
{
	$str = str_par($var, null);
	if (!$str)
		return $default;
	
	elseif (preg_match('/^[0-9]+$/', $str))
		return (int)$str * 100;
		
	elseif (preg_match('/^([0-9]*)[,.]([0-9]{2})$/', $str, $regs))
		return (int)$regs[1] * 100 + (int)$regs[2];

	elseif (preg_match('/^([0-9]*)[,.]([0-9])$/', $str, $regs))
		return (int)$regs[1] * 100 + (int)$regs[2] * 10;

	else
		return $default;
}

function op_par($default = 'SHOW')
{
	$allowed = array('SHOW', 'MOD', 'MOD2', 'NEW', 'NEW2', 'DEL', 'DEL2');
	$op = str_par('op');
	if (in_array($op, $allowed))   #standardowe
		return $op;
	elseif ($op && strtolower($op)==$op)  #niestandardowe - male litery
		return $op;
	else
		return $default;
}


function pstr($str, $desc = null)
{
	global $L_debug_IPs;	
	if (in_array($_SERVER['REMOTE_ADDR'], $L_debug_IPs))
	{
		echo "<pre>\n.---- $desc\n";
		ob_start('htmlspecialchars');
		print_r($str);
		ob_end_flush();
		echo "\n`-----\n</pre>\n";
	}
}

function valid_date($data)
{
	if (!ereg("^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})$", $data, $regs))
		return false;
	
	return checkdate($regs[2], $regs[3], $regs[1]);
}

# data musi byc zwalidowana
function date2sql($data)
{
	if ($date)
		return "to_date('$data', 'yyyy-mm-dd')";
	else
		return "NULL";
}

# 12345 -> 123.45
function cent2str($cents)
{
	return sprintf("%d.%02d", $cents/100, $cents%100);
}


?>
