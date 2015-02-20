<?

function view($vp)
{
	if (!is_array($vp))
		echo htmlspecialchars($vp);
	else switch($vp['type'])
	{
		case 't': # template
			$C = &$vp['obj'];
			include('templates/'.$vp['content']).'.inc';
		break;

		case 'h': # HTML
			echo $vp['content'];
		break;
		
		case 'u': # URL
			if (is_array($vp['obj']))
			{
				if (strpos($vp['content'], '?') !== false)
					$sep = '&';
				else
					$sep = '?';

				$opts = '';
				foreach ($vp['obj'] as $k => $v)
				{
					$opts .= $sep.urlencode($k).'='.urlencode($v);
					$sep = '&';
				}
			}
			else
				$opts = '';
			echo htmlspecialchars($vp['content'].$opts);

		break;

		case 'l': # LINK <a>
			$params = '';
			if (is_array($vp['obj']))
				foreach ($vp['obj'] as $k => $v)
					$params .= ' '
						.htmlspecialchars($k)
						.' = "'
						.htmlspecialchars($v)
						.'"';

			echo '<a href="';
			view($vp['content']['url']);
			echo '"';
			echo htmlspecialchars($params);
			echo '>';
			view($vp['content']['txt']);
			echo '</a>';
		break;

		case 'f':
#			echo $($vp['content'])($vp['obj']);
		break;
	}
}


function mkrawview($type, $content, $obj = null)
{
	return array('type' => $type, 'content' => $content, 'obj' => $obj);
}

# ex types:
# t:template (for template.inc)
# f:func (for func($subobj))
# h:<html>code</html>
function mkview($pattern, $obj = null)
{
	list($type,$content) = explode(':', $pattern, 2);
	return mkrawview($type, $content, $obj);
}

#  mkulink(mkview('u:link.php',array('param' => 'value'), array('class' => 'aclass')), 'Kliknij tu');
function mkulink($url, $txt, $params = null)
{
	return mkrawview('l', array('url' => $url, 'txt' => $txt), $params);
}

# mklink('Kliknij tu', 'link.php', array('param' => 'value'))
function mklink($txt, $url, $urlparams = array(), $params = null)
{
	return mkulink(mkview('u:'.$url, $urlparams), $txt, $params);
}

function mkselect($slownik, $sel = -1, $zzerem = false)
{
	$sel = array();
	foreach ($slownik as $k => $v)
		if ($k || !$zzerem)
			$sel[$k] = $v;

	return $sel;
}

# adds message to url
# msg transported via _mainmsg parameter and viewed
function url_add_msg($url, $msg, $variable='_mainmsg')
{
	$msg = urlencode($msg);
	if (strpos($url, '?') === false)
		return "$url?$variable=$msg";
	else
		return "$url&$variable=$msg";
}

?>
