import React, { Component } from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';
import { NavLink } from 'react-router-dom';

import Home from './components/Home';
import Page1 from './components/Page1';
import Page2 from './components/Page2';
import Error from './components/Error';
import Navigation from './components/Navigation';
 
class App extends Component {
  render() {
    return (      
       <BrowserRouter>
        <div>
        
          <Navigation />
            <Switch>
             <Route path="/" component={Home} exact/>
             <Route path="/CenterArt" component={Page1}/>
             <Route path="/CropArt" component={Page2}/>
            <Route component={Error}/>
           </Switch>
        </div> 
      </BrowserRouter>
    );
  }
}
 
export default App;