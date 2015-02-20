<?

function helper_opts($op)
{
	foreach ($op['opts'] as $k => $v)
	{
		if ($k == $op['selected'])
			echo "<option value=\"$k\" selected>";
		else
			echo "<option value=\"$k\">";
		view($v);
		echo "</option>\n";
	}
}

?>
