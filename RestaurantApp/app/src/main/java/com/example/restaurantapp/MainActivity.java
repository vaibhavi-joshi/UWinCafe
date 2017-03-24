package com.example.restaurantapp;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.Socket;
import android.os.StrictMode;
import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import android.widget.EditText;


public class MainActivity extends ActionBarActivity {
    private boolean isRunning = false;
    private String serverIpAddress = "192.168.1.141";
    final int SERVER_PORT = 8935;
    TextView textView;
    Button b1;
    BufferedReader input;
    Socket socket = new Socket();
    boolean doExit=true;
    String st;
    StringBuffer menuSt=new StringBuffer();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = (TextView)findViewById(R.id.textView);

       // btnService = (Button)findViewById(R.id.btnService);
       // btnService.setOnClickListener(new View.OnClickListener() {
          //  @Override
          //  public void onClick(View view) {
                if (!isRunning) {
                    try {
                        if (android.os.Build.VERSION.SDK_INT > 9) {
                            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
                            StrictMode.setThreadPolicy(policy);
                        }

                        Toast toast=Toast.makeText(MainActivity.this, serverIpAddress.toString(), Toast.LENGTH_LONG);
                        toast.setGravity(Gravity.CENTER, 0, 0);
                        toast.show();

                        socket.connect(new InetSocketAddress(serverIpAddress, SERVER_PORT), 5000);

                        //Socket socket = new Socket(serverAddress, SERVER_PORT);
                        input =  new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        PrintWriter out = new PrintWriter(new BufferedWriter(
                                new OutputStreamWriter(socket.getOutputStream())), true);
                        String st;
                        StringBuffer menuSt=new StringBuffer();


                        while(true)
                        {
                            st = input.readLine();
                            if(st.compareTo("")==0) {
                                break;
                            }

                            System.out.println(st);
                            menuSt.append(st+"\n");
                            st = null;
                        }
                        textView.setText(""+menuSt);
                        isRunning = true;
                    } catch (Exception e) {
                        Toast toast=Toast.makeText(MainActivity.this, e.getMessage(), Toast.LENGTH_LONG);
                        toast.setGravity(Gravity.CENTER, 0, 0);
                        toast.show();

                        e.printStackTrace();

                        isRunning = false;
                    }
                }
         //   }
       // });
        b1=(Button)findViewById(R.id.button2);
        b1.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v){

                    try {

                        PrintWriter out = new PrintWriter(new BufferedWriter(
                                new OutputStreamWriter(socket.getOutputStream())), true);

                        EditText txtSelection = (EditText) findViewById(R.id.txtSelection);
                        String txt = txtSelection.getText().toString();



                        out.println(txt);

                        input = new BufferedReader(new InputStreamReader(socket.getInputStream()));

                        String st;
                        StringBuffer menuSt = new StringBuffer();


                       if (textView.getText().toString().contains("continue") && txt.compareTo("1") == 0) {

                                //print menu
                            while(true)
                            {
                                st = input.readLine();
                                if(st.compareTo("")==0) {
                                    break;
                                }

                                System.out.println(st+" \n");
                                menuSt.append(st);
                                st = null;
                            }
                            textView.setText(""+menuSt);
                           txtSelection.getText().clear();
                        }
                       else {

                           st = input.readLine();
                           textView.setText("" + st);

                           //if(st.contains("total"))
                           if(st.contains("Check"))
                           {
                               txtSelection.setVisibility(View.INVISIBLE);
                               b1.setVisibility(View.INVISIBLE);
                           }
                           txtSelection.getText().clear();
                        }


                    } catch (Exception e) {
                        Toast toast = Toast.makeText(MainActivity.this, e.getMessage(), Toast.LENGTH_LONG);
                        toast.setGravity(Gravity.CENTER, 0, 0);
                        toast.show();

                        e.printStackTrace();
                    }
            }
        });
    }


    public void onDestroy(){
        super.onDestroy();
        // stop background Thread
        isRunning = false;
    }




    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
       // if (id == R.id.action_settings) {
         //   return true;
        //}

        return super.onOptionsItemSelected(item);
    }

}