import React, { Component } from 'react';
import { Radio, Input, Button} from 'antd';
import axios from 'axios';

class Page1 extends Component {
  constructor(props) {
    super(props);
    this.state = { x: 0, y: 0 , xcl: 0, ycl: 0, ximp: 0, yimp: 0, value: 1};
  }
  state = {
    value: 1,
    ximp: 0,
    yimp: 0,
  };

  
  onChange = e => {
    console.log('radio checked', e.target.value);
    this.setState({
      value: e.target.value,
    });
  };
  toggleDisable = () => {
    this.setState({disabled: !this.state.disabled});
    if(this.state.disabled == true){
      this.setState({ximp: "0", yimp: "0"})
    }
  };
updateInputValueX(e) {
    this.setState({ximp: e.target.value == "" ? 0 : parseFloat(e.target.value)});
    
  }
  updateInputValueY(e) {
    this.setState({yimp: e.target.value == "" ? 0 : parseFloat(e.target.value)});
    
  } 
  _onMouseMove(e) {
    this.setState({ x: e.screenX - 640, y: e.screenY - 230 });
  }

  submit = (e) => {
    const {ximp, yimp} = this.state;
    const coords = {ximp, yimp};
    axios
      .post('http://localhost:3001/create', coords)
      .then(() => console.log('Book Created'))
      .catch(err => {
        console.error(err);
      });
  };
  
  _onClick(e){
    this.setState({ xcl: e.screenX - 640, ycl: e.screenY - 230 });
  }

  moveLeft = (e) => {
    this.setState({ximp: this.state.ximp - 1});
  };
  moveRight = (e) => {
    this.setState({ximp: this.state.ximp + 1});
  }
  moveUp = (e) => {
    this.setState({yimp: this.state.yimp + 1});
  }
  moveDown = (e) => {
    this.setState({yimp: this.state.yimp - 1});
  }
  render() {
    const { x, y, xcl, ycl, ximp, yimp} = this.state;
    return (
      <div className = "App">
      <div >
        <img onMouseMove={this._onMouseMove.bind(this)} onClick = {this._onClick.bind(this)} src={require('./art.jpg')} className = "image" />
      </div>
      <h2>Coordinates of Mouse: {x} {y}</h2>
      <h2>Chosen Coordinates: {xcl} {ycl}</h2>
      <h2>Coords of Center: {ximp} {yimp}</h2>
      <Radio.Group onChange={this.onChange} value={this.state.value}>
        <Radio value={1} onChange = {this.toggleDisable}>Center of Video Stream</Radio>
        <Radio value={2} onChange = {this.toggleDisable}>Manually Enter Center</Radio>
      </Radio.Group>
      <div className = "example-input">
      <Input placeholder = "X Coordinate" disabled = {!this.state.disabled} value = {this.state.ximp} onChange = {this.updateInputValueX.bind(this)}/>
      <Input placeholder = "Y Coordinate" disabled = {!this.state.disabled} value = {this.state.yimp} onChange = {this.updateInputValueY.bind(this)}/>  
      </div>
      
      <div className = "move-buttons">    
      <Button onClick = {this.moveUp}> Move Up </Button>
      <Button onClick = {this.moveDown}> Move Down </Button>
      <Button onClick = {this.moveLeft}> Move Left </Button>
      <Button onClick = {this.moveRight}> Move Right </Button>

      </div>
      <div>
        <Button className = "move-buttons" type = 'primary' onClick = {this.submit}> Submit Coords </Button>
      </div>
      </div>
    );
  }
}
 
export default Page1;