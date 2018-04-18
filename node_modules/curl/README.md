# CURL -- Simple client url library, with high level request functions

## Install

<pre>
  npm install curl
</pre>

Or from source:
<pre>
  git clone git://github.com/ianjorgensen/curl.git 
  cd curl
  npm link .
</pre>

## Functions
### curl.get

<pre>curl.get(url, options, function(err, response, body) {});</pre>
	
### curl.post

<pre>curl.post(url, body, options, function(err, response, body) {});</pre>

### curl.getJSON

<pre>curl.getJSON(url, options, function(err, response, data){});</pre>

### curl.postJSON

<pre>curl.postJSON(url, data, options, function(err, response, data){});</pre>