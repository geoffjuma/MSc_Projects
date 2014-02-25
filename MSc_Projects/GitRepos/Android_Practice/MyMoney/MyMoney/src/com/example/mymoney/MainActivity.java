package com.example.mymoney;

import android.os.Bundle;
import android.app.Activity;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {

	Button btnLogin, btnCancel;
	EditText logEmail, logPassword;
	TextView toRegister;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setInitializer();
        
        btnLogin.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
			}
		});
        
        btnCancel.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
				logEmail.setText("");
				logPassword.setText("");
				finish();
			}
		});
        toRegister.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
			
				Intent goToRegister = new Intent(getApplicationContext(),Register.class);
				startActivity(goToRegister);
				finish();
			}
		});
    }


    private void setInitializer() {
		// TODO Auto-generated method stub
    	btnLogin = (Button)findViewById(R.id.log_Login);
    	btnCancel = (Button)findViewById(R.id.log_Cancel);
    	logEmail = (EditText)findViewById(R.id.log_email);
    	logPassword = (EditText)findViewById(R.id.log_password);
    	toRegister = (TextView)findViewById(R.id.link_to_register);
    	
	}


	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }


    
}
