package com.xindawn.util;

import android.content.Context;
import android.preference.Preference;
import android.view.View;
import com.xindawn.R;

public class PreferenceHead extends Preference {

	public PreferenceHead(Context context) {
		super(context);
		setLayoutResource(R.layout.preference_head);
	}

	@Override
	protected void onBindView(View view) {
		super.onBindView(view);
	}
}
