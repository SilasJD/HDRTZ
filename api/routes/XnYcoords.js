const express = require('express');
const logger = require('morgan');
const cors = require('cors');
const fs = require('fs') 

const app = express();

//use cors to allow cross origin resource sharing
app.use(
  cors({
    origin: 'http://localhost:3000',
    credentials: true,
  })
);

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));

let books = [];

app.post('/create', function(req, res) {
  const coords = {
    x: req.body.ximp,
    y: req.body.yimp}

  console.log(coords);
  fs.writeFile('output.json', JSON.stringify(coords), (err) => { 
      
    // In case of a error throw err. 
    if (err) throw err; 
    }) 
    res.send(200);
});

//start your server on port 3001
app.listen(3001, () => {
  console.log('Server Listening on port 3001');
});

module.exports = app;
