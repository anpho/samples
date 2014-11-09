/*
 *
 * dataURI to Local File function , for webkit ( HTML5 ) browsers
 * All rights reserved by anphorea@gmail.com, obtain an Email confirmation before you use.
 * 
 * USAGE:
 * var data="data:image/jpeg;base64,/........";
 * dataURItoFile(data,function(e){console.log("File Path:"+e)})
 */
 
function dataURItoFile(dataURI, callback) {
  // Convert dataURI to binary file content.
	var byteString = atob(dataURI.split(',')[1]);
	var mimeString = dataURI.split(',')[0].split(':')[1].split(';')[0];
	var ab = new ArrayBuffer(byteString.length);
	var ia = new Uint8Array(ab);
	for (var i = 0; i < byteString.length; i++) {
		ia[i] = byteString.charCodeAt(i);
	}
	var dataview = new DataView(ab);
	var bb = new Blob([dataview], {
		type: mimeString
	});
	// bb is file content.


	window.webkitRequestFileSystem(window.TEMPORARY, 5 * 1024 * 1024, function(fs) {
		console.log('Opened file system: ' + fs.name);
		fs.root.getFile(new Date().getTime() + '.png', {
			create: true,
			exclusive: true
		}, function(fileEntry) {
			var fe = fileEntry;
			fe.createWriter(function(writer) {
				var fw = writer;
				fw.onwriteend = function() {
					console.log('write end')
					callback(fe.toURL());
				}
				fw.write(bb);
			}, function(e) {
				callback(null);
			})
		}, function(e) {
			callback(null);
		});
	}, function(e) {
		callback(null);
	});
}

/*
 * For @zhetengbj , to save canvas dataURI to local file and callback the file URL.
 * /
 
