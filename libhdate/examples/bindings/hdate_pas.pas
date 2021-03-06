(* hdate_pas.pas
 * example file for libhdate.
 *)

(*
 compile:  gpc --automake hdate_pas.pas --cstrings-as-strings -lhdate -o hdate_pas
 *)
 
(*
 *  Copyright (C) 2008  Yaacov Zamir <kzamir@walla.co.il>
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *)
 
program hdate_pas;

uses hdate;

var
  h: Phdate_struct; { date will be stored in h }
  d, m, y: Integer;
  
begin
  { alocate the hdate memory }
  h := new_hdate ();
  
  { copy the hebrew date struct of today <day=0, month=0, year=0> }
  h := hdate_set_gdate(h, 0, 0, 0);
  
  { print gregorian date }
	writeLN ('Today is :');
	
  d := hdate_get_gday (h);
	m := hdate_get_gmonth (h);
	y := hdate_get_gyear (h);
  writeLN (d, ',', m , ',', y);
	
	{ print hebrew date: 0 - israely holidays, 0 - long format }
	writeLN (hdate_get_format_date (h, 0, 0));
	
	{ unref the used memory }
  h := delete_hdate(h);
end.

