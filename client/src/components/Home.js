import React from 'react';

import Button from 'antd/es/button';
import {Menu} from 'antd';
import './App.css';
import { NavLink } from 'react-router-dom';
import Page1 from './Page1';
import { BrowserRouter, Route, Switch } from 'react-router-dom';


const home = () => {
    return (
       <div className="App">
          
              <h2 
              className = "welcome">
              Welcome to HDRTZ WebApp </h2>
              <Button 
              type="primary" 
              className = "button1"
              href = "http://localhost:3000/CenterArt"
              >
            Center Artworks
              </Button>
              <img src={require('./art.jpg')} className = "image" />

              <Button 
              type="primary" 
              className = "button2"
              href = "http://localhost:3000/CropArt"
              >
              Crop Artworks
              </Button>
    </div>  
    );
}

export default home;