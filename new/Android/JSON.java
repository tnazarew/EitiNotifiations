package com.example.maksimandrzejewski.first;
import android.util.Log;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

/**
 * Created by Maksim Andrzejewski on 08-May-16.
 */
public class JSON {

    JSONParser parser = new JSONParser();
    final String LOG_TAG = "JSONlogs";

    JSON(){}

    public String decodeMessages(String json) throws ParseException {
        Object obj = parser.parse(json);
        JSONObject jsonObj = (JSONObject) obj;
        return (String) jsonObj.get("message");
    }
}
