//
// Copyright (C) 2006 Mekensleep
//
// Mekensleep
// 24 rue vieille du temple
// 75004 Paris
//       licensing@mekensleep.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA.
//
// Authors:
//  Loic Dachary <loic@gnu.org>
//

function set_update_timeout(url, delay) {

    var update_matrix = function(transport, object) {
	alert(transport.responseText);
    }

    var check_update = function() {
	var schedule = new Ajax.Request( url + '/updates.php', { method: 'get',
								 parameters: 'timestamp=' + timestamp,
								 evalScripts: true,
								 //onComplete: update_matrix,
	    });
    }

    setTimeout(check_update, delay);
}

function schedule(url, yesno, id, package, distribution) {
    var params = 'package=' + encodeURIComponent(package) + '&distribution=' + encodeURIComponent(distribution) + '&' + yesno;
    var lambda = function(transport, object) {
	update_package_distribution(url, id, package, distribution);
    };
    var schedule = new Ajax.Request( url + '/schedule.php', { method: 'get', parameters: params, onComplete: lambda });

    set_update_timeout(url, 5000);
}

function update_package_distribution(url, id, package, distribution) {
    var params = 'package=' + encodeURIComponent(package) + '&distribution=' + encodeURIComponent(distribution);

    var lambda = function(transport, object) {
	var receiver = $(id);

	receiver.onmouseover = receiver.childNodes[0].onmouseover;
	receiver.onmouseout = receiver.childNodes[0].onmouseout;
	receiver.className = receiver.childNodes[0].className;
    };

    var updater = new Ajax.Updater(id, url + '/package_distribution.php',
				   { method: 'get', 
				     parameters: params,
				     onComplete: lambda });
}

function details_hide() {
    $('details').innerHTML = '<p>';
}

function details(url, package, distribution) {
    var params = 'package=' + encodeURIComponent(package) + '&distribution=' + encodeURIComponent(distribution);

    var updater = new Ajax.Updater('details',
				   url + '/details.php',
				   { method: 'get', 
				     parameters: params });
}
