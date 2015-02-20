<?
function helper_table_th($C, $titles) #OBSOLETE
{
	#pstr($C['hdrs']);
	foreach ($C['hdrs'] as $h)
	if (isset($h['title']))
	{
		echo '<th class="tabela">';
		view($h['title']);
		echo ' <a href="';
		view($h['urlasc']);
		echo '">asc</a> <a href="';
		view($h['urldesc']);
		echo '">desc</a>';
		echo '</th>';
	}
}

function helper_tabela_updown($C, $n)
{
	typassert($C, 'class Tabela');
	$h = $C['hdrs'][$n-1];
	
	$actdown = $actup = '';
	if (isset($h['sort']))
	{
		if ($h['sort'] == 'a')
			$actdown = 'b';
		elseif ($h['sort'] == 'd')
			$actup = 'b';
	}
	
	echo '<a href="';
	view($h['urlasc']);
	echo '"><img src="gfx/'.$actdown.'down.png" class="tabela" alt="V"/></a><a href="';
	view($h['urldesc']);
	echo '"><img src="gfx/'.$actup.'up.png" class="tabela" alt="^"/></a>';
}

function helper_tabela_strony($C)
{
	#pstr($C);
	typassert($C, 'class Tabela');
	foreach ($C['strony'] as $link)
		{ view($link); echo '&nbsp;'; };
}

?>
