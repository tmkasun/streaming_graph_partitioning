import React from 'react';
import {
  Switch,
  Route,
  BrowserRouter,
} from 'react-router-dom';

import CssBaseline from '@material-ui/core/CssBaseline';
import { ThemeProvider } from '@material-ui/core/styles';

import Welcome from './pages/Welcome';
import Base from './pages/Base';
import theme from './theme/theme';

export default function Routing() {
  return (
    <BrowserRouter>
      <ThemeProvider theme={theme}>
        <CssBaseline />
        <Base>
          <Switch>
            <Route exact path="/welcome">
              <Welcome />
            </Route>
            <Route exact path="/">
              <Welcome />
            </Route>
          </Switch>
        </Base>
      </ThemeProvider>
    </BrowserRouter>
  );
}
