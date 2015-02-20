<?

function helper_forms_warnrow($C, $field)
{
	if (isset($C['warn'][$field]))
	{
		echo '<tr><td colspan="2" class="mod-warn">';
		view($C['warn'][$field]);
		echo "</td></tr>\n";
	}
}

# na ogół name = field
function helper_forms_input($C, $txt, $field, $name = null)
{
	if (!$name)
		$name = $field;
	echo '<tr><td class="mod-desc">'.$txt.'</td><td class="mod-field"><input type="text" size=60 name="'.$name.'" value="';
	if (isset($C[$field]))
		view($C[$field]);
	echo "\"/></td></tr>\n";
	helper_forms_warnrow($C, $field);
}

# <textarea cols="xx" rows="xx"> .... </textarea>
function helper_forms_textarea($C, $txt, $field, $rows = 10, $cols = 60, $name = null)
{
	if (!$name)
		$name = $field;
	echo '<tr><td class="mod-desc">';
	view($txt);
	echo '</td><td class="mod-field"><textarea name="'.$name
		.'" rows="'.$rows.'" cols="'.$cols.'">';
	if (isset($C[$field]))
		view($C[$field]);
	echo "</textarea></td></tr>\n";
	helper_forms_warnrow($C, $field);
}

# <select>
#     <option name="xx"> ... </option>
# </select>
function helper_forms_input_select($C, $txt, $field, $opts, $selected = null)
{
	if (empty($selected))
		$selected = $C[$field];
	echo '<tr><td class="mod-desc">'.$txt.'</td><td class="mod-field"><select name="'.$field.'">';
	foreach ($opts as $k => $opt)
	{
		if (!$opt)
			continue;
		if ($k == $selected)
			echo "<option value=\"$k\" selected>";
		else
			echo "<option value=\"$k\">";
		view($opt);
		echo "</option>\n";
	}
	echo "</select></td></tr>\n";
	helper_forms_warnrow($C, $field);
}

?>
