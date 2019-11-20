import React from 'react';
import Home from "./Home";
import { BrowserRouter as Router, Route, Link } from "react-router-dom";
import {Menu, Icon, Switch} from 'antd';
const { SubMenu } = Menu;



function handleClick(e) {
  console.log('click', e);
}

class Navigation extends React.Component{
    state = {
    theme: 'dark',
    current: '1',
  };

  changeTheme = value => {
    this.setState({
      theme: value ? 'dark' : 'light',
    });
  };

  handleClick = e => {
    console.log('click ', e);
    this.setState({
      current: e.key,
    });
  };

  render() {
    return (
      <div className = 'NavBar'>
        <Switch
          checked={this.state.theme === 'dark'}
          onChange={this.changeTheme}
          checkedChildren="Dark"
          unCheckedChildren="Light"
        />
        <br />
        <br />
        <Menu
          theme={this.state.theme}
          onClick={this.handleClick}
          style={{ width: 256 }}
          defaultOpenKeys={['sub1']}
          selectedKeys={[this.state.current]}
          mode="inline"
          position='relative'
        >
          <SubMenu
            key="sub1"
            title={
              <span>
                <Icon type="mail" />
                <span>Home</span>
              </span>
            }
          >
            <Menu.Item key="1">Go Home</Menu.Item>
              
            <Menu.Item key="2">Edit Rotational Values</Menu.Item>
            
          </SubMenu>
          <SubMenu
            key="sub2"
            title={
              <span>
                <Icon type="appstore" />
                <span>Center Art</span>
              </span>
            }
          >
            <Menu.Item key="3" href = "http://localhost:3000/CenterArt">Choose Center by Image</Menu.Item>
            <Menu.Item key="4" href = "http://localhost:3000/CenterArt">Enter Center Manually</Menu.Item>
            
          </SubMenu>  
          <SubMenu
            key="sub4"
            title={
              <span>
                <Icon type="setting" />
                <span>Crop Art</span>
              </span>
            }
          >
            <Menu.Item key="9" href = "http://localhost:3000/CropArt">Crop Art Around an Image</Menu.Item>
          </SubMenu>
        </Menu>
      </div>
    );
  }
}
 
export default Navigation;