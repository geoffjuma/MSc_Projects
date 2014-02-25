package com.example.mymoney;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class Register extends Activity{
	
	Button btnRegister;
	EditText regPassword, regUserName, regEmail;
	TextView toLogin;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.register);
		setInitializer();
		
		btnRegister.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				String name = regUserName.getText().toString();
				String password = regPassword.getText().toString();
				String email = regEmail.getText().toString();
				
			}
		});
		toLogin.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
				Intent backToLogin = new Intent(getApplicationContext(), MainActivity.class);
				startActivity(backToLogin);
			}
		});
		
	}

	private void setInitializer() {
		// TODO Auto-generated method stub
		btnRegister = (Button)findViewById(R.id.btnRegister);
		regPassword = (EditText)findViewById(R.id.reg_password);
		regUserName = (EditText)findViewById(R.id.reg_fullname);
		regEmail = (EditText)findViewById(R.id.reg_email);
		toLogin = (TextView)findViewById(R.id.link_to_login);
	}
	

}
