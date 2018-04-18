var server = require('router').create();

var toJSON = function(request, callback) { 
	var res = '';
	
	request.on('data', function (chunk) { 
		res += chunk; 
	});
	request.on('end',function() { 
		try { 
			res = JSON.parse(res);
		} 
		catch(err) { 
			callback(err); 
			return; 
		} 
		callback(null, res); 
	}); 
};
	
server.get('/json', function(request, response) {
	response.end(JSON.stringify({hello:'world'}))
});

server.get('/post', function(request, response) {
	console.log('post get');
	response.end('hi');
});

server.post('/post', function (request, response) {
	console.log('hit post');
	toJSON(request, function(err, data) {
		if (err) {
			console.error(err.message,err.stack);
			return;
		}
		console.log(data);
		response.end('that\'s all folks \n');
	})
});

server.listen(10001);