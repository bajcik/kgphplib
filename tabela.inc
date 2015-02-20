<?

include_once 'helpers/tabela.inc';

class Tabela
{
	var $nazwa;
	var $cols;
	var $offset = 0;
	var $limit = 100;
	var $nall;       # liczba wierszy wyniku (bez offset/limit)
	var $n;          # liczba wierszy wyniku (z offset/limit)
	var $stron;
	var $strona;     # od 0!
	var $order;      # 'a3,d1,d2'
	var $ordertab;   # {3,true}{1,false}{2,false}
	var $tab;        # 2wymiarowa tablica z zawartoscia
	var $hdrs;       # tablica array(sort (a/d), urlasc, urldesc)
	var $strony;     # lista stron-linkow (od [1]! nr => link

	function Tabela($nazwa, $sql, $order = 'a1,a2')
	{
		$this->nazwa = $nazwa;

		# parametry tablicy
		$this->offset = int_par($nazwa.'_of', 0);
		$this->limit = int_par($nazwa.'_li', $this->limit);
		$this->order = str_par($nazwa.'_or', $order);

		# zapytanie "badawcze"
		$k = sqlex($sql);
		$this->nall = sql_numrows($k);
		$this->cols = sql_numfields($k);
		sql_free($k);

		$this->hdrs = array();
		for ($i=0; $i<$this->cols; $i++)
			$this->hdrs[$i] = array();

		# rachunek stron
		if ($this->offset >= $this->nall) max(0, $this->offset = $this->nall - 1);
		if ($this->offset <0) $this->offset = 0;
		$this->stron = (int)(($this->nall + $this->limit - 1) / $this->limit);
		$this->strona = (int)($this->offset / $this->limit);
		
		#domyslny link do aktualnego stanu
		#$this->url = mkview("u:?",array(
		#	$nazwa.'_of' => $this->offset,
		#	$nazwa.'_li' => $this->limit,
		#	$nazwa.'_or' => $this->order,
		#));
		$this->url = mkview("u:?", $_GET);
		$this->url[$nazwa.'_of'] = $this->offset;
		$this->url[$nazwa.'_li'] = $this->limit;
		$this->url[$nazwa.'_or'] = $this->order;

		$pokazane = array();
		# linki poczatkowych stron
		for ($i=0; $i<5 && $i<$this->stron; $i++)
		{
			$this->strony[] = $this->mklink_strona($i);
			$pokazane[$i] = true;
		}
		if ($this->strona > 7)
			$this->strony[] = ' ... ';
		# linki sasiednich stron
		for ($i=max(0, $this->strona-2); $i<=$this->strona+2 && $i<$this->stron; $i++)
			if (!isset($pokazane[$i]))
			{
				$this->strony[] = $this->mklink_strona($i);
				$pokazane[$i] = true;
			}
		$this->strony[] = ' ... ';
		# linki ostatnich stron
		for ($i=max(0, $this->stron-5); $i<$this->stron; $i++)
			if (!isset($pokazane[$i]))
			{
				$this->strony[] = $this->mklink_strona($i);
				$pokazane[$i] = true;
			}
		# linki << >>
		$this->strony[] = $this->mklink_strona(max(0, $this->strona-1), '<<');
		$this->strony[] = $this->mklink_strona(min($this->strona+1, $this->stron-1), '>>');

		# sortowanie
		$this->ordertab = array();
		$ordarr = explode(',', $this->order);
		$ordersql = ''; $prz = '';
		$first = true;
		foreach ($ordarr as $c)
		{
			$asc = (substr($c, 0, 1) == 'a');
			$ncol = (int)substr($c, 1) -1; #FIXME
			
			if ($ncol < 0 || $ncol > $this->cols || isset($this->hdrs['sort']))
				continue;

			if ($first)
				$this->hdrs[$ncol]['sort'] = $asc?'a':'d';
			$this->ordertab[] = array($ncol, $asc);

			$ordersql .= $prz.($ncol+1).($asc ? ' asc':' desc'); $prz=', ';
			$first = false;
		}

		# uzupelnienie info naglowkow kolumnowych
		for ($i=0; $i<$this->cols; $i++)
		{
			$this->hdrs[$i]['urlasc'] = $this->mkurl_order($i, true);
			$this->hdrs[$i]['urldesc'] = $this->mkurl_order($i, false);
		}

		# zapytanie wlasciwe
		$this->tab = sql2assoc($sql." order by $ordersql limit {$this->offset},{$this->limit}");
		$this->n = count($this->tab);
	}

	function mklink_strona($n, $str = null)
	{
		if (!$str)
			$str = '['.($n+1).']';
		if ($n == $this->strona)
			return $str;
		$ofs = $n*$this->limit;
		$url2 = $this->url;
		$url2['obj'][$this->nazwa.'_of'] = $ofs;
		$l = mklink($str, '?', $url2['obj']);
		return $l;
	}

	function mkurl_order($k, $asc)
	{
		$order = ($asc?'a':'d').($k+1);
		foreach ($this->ordertab as $t)
			if ($t[0] != $k)
				$order .= ','.($t[1]?'a':'d').($t[0]+1);

		$url2 = $this->url;
		$url2['obj'][$this->nazwa.'_or'] = $order;
		return $url2;
	}

	function genarray()
	{
		return array(
			'typassert' => 'class Tabela',
			'hdrs' => $this->hdrs,
			'strona' => $this->strona,
			'strony' => $this->strony,
			'tab' => $this->tab
		);
	}
	
	function genview($templ)
	{
		return mkview("t:$templ", $this->genarray());
	}

	function rekordy()
	{
		return $this->tab;
	}
}

?>
