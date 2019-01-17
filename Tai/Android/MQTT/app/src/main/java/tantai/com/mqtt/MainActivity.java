package tantai.com.mqtt;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;


import java.io.UnsupportedEncodingException;
import java.util.Calendar;

import static java.lang.Long.parseLong;

public class MainActivity extends AppCompatActivity {
    private final long ELEC_PRICE = 3000;
    private final long WATER_PRICE = 7000;
    private final long ROOM_PRICE = 1200000;
    private Button btn, btnLoadData;
    private ToggleButton btnOnOff;
    private TextView tvWtCurr, tvEltCurr, tvWtPre, tvEltPre, tvCurrMonth, tvPreMonth, tvSum;
    private RadioGroup rdgSelect;
    private EditText etRoom, etTime;
    MqttAndroidClient client;
    private Calendar cal;
    private int currMonth, preMonth;
    private String room = "0";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findView();
        connect();

        cal = Calendar.getInstance();
        currMonth = cal.get(Calendar.MONTH) + 1;
        preMonth = (currMonth == 1) ? 12 : currMonth - 1;
        tvCurrMonth.setText("Chỉ số tháng " + currMonth);
        tvPreMonth.setText("Chỉ số tháng " + preMonth);


        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (rdgSelect.getCheckedRadioButtonId() == R.id.rb_time) {
                    if (!etRoom.getText().toString().equals("") && !etTime.getText().toString().equals("")) {
                        String command = "OFF!" + etRoom.getText() + "!" + etTime.getText();
                        publish(client, command, "NguonDienTong");
                        Toast.makeText(getApplication(), "Đã thực hiện", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(getApplication(), "Bạn cần nhập đầy đủ thông tin", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
        btnOnOff.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                String temp = etRoom.getText().toString();
                if (etRoom.getText().toString().equals("")) {
                    Toast.makeText(getApplication(), "Bạn cần nhập số phòng", Toast.LENGTH_SHORT).show();
                    btnOnOff.setChecked(false);
                } else {
                    String command;
                    if (btnOnOff.getText().equals("Bật")) {
                        command = "OFF!";
                    } else {
                        command = "ON!";
                    }
                    publish(client, command, etRoom.getText().toString() + "/NguonDienTong");
                    Toast.makeText(getApplication(), "Đã thực hiện", Toast.LENGTH_SHORT).show();
                }
            }
        });
        btnLoadData.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (etRoom.getText().toString().equals("")) {
                    Toast.makeText(getApplication(), "Bạn cần nhập số phòng", Toast.LENGTH_SHORT).show();
                } else {
                    room = etRoom.getText().toString();
                    connect();
//                    if (tvWtPre.getText().equals("0") && tvEltPre.getText().equals("0") &&
//                            tvWtCurr.getText().equals("0") && tvEltCurr.getText().equals("0")) {
//                        Toast.makeText(getApplication(), "Không có dữ liệu", Toast.LENGTH_SHORT).show();
//                    }
                }
            }
        });
        rdgSelect.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                int selectedId = rdgSelect.getCheckedRadioButtonId();
                if (selectedId == R.id.rb_manual) {
                    btnOnOff.setVisibility(View.VISIBLE);
                    etTime.setVisibility(View.GONE);
                    btn.setVisibility(View.GONE);
                } else {
                    btnOnOff.setVisibility(View.GONE);
                    etTime.setVisibility(View.VISIBLE);
                    btn.setVisibility(View.VISIBLE);
                }
            }
        });
    }

    public void connect() {
        String clientId = MqttClient.generateClientId();
        client = new MqttAndroidClient(this.getApplicationContext(), "tcp://m21.cloudmqtt.com:14531", clientId);

        MqttConnectOptions options = new MqttConnectOptions();
        options.setMqttVersion(MqttConnectOptions.MQTT_VERSION_3_1);
        options.setCleanSession(false);
        options.setUserName("girlnujd");
        options.setPassword("uzlMkPzoIxcs".toCharArray());

        try {
            IMqttToken token = client.connect(options);
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {

                    subscribe(client, room + "/" + currMonth);
                    subscribe(client, room + "/" + preMonth);

                    tvEltCurr.setText("0");
                    tvWtCurr.setText("0");
                    tvEltPre.setText("0");
                    tvWtPre.setText("0");
                    tvSum.setText("0");

                    client.setCallback(new MqttCallback() {
                        @Override
                        public void connectionLost(Throwable cause) {
                        }

                        @Override
                        public void messageArrived(String topic, MqttMessage message) throws Exception {
                            message.getPayload();
                            String[] temp = sliptString(message.toString());
                            if (topic.equals(room + "/" + currMonth)) {
                                tvEltCurr.setText(temp[0]);
                                tvWtCurr.setText(temp[1]);
                            } else if (topic.equals(room + "/" + preMonth)) {
                                tvEltPre.setText(temp[0]);
                                tvWtPre.setText(temp[1]);
                            }

                            if (!tvWtPre.getText().equals("0") && !tvEltPre.getText().equals("0") &&
                                    !tvWtCurr.getText().equals("0") && !tvEltCurr.getText().equals("0")) {
                                tvSum.setText(calculateSum() + "");
                                Toast.makeText(getApplication(), "Đã tải xong", Toast.LENGTH_SHORT).show();
                            }
                        }

                        @Override
                        public void deliveryComplete(IMqttDeliveryToken token) {
                        }
                    });
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void publish(MqttAndroidClient client, String payload, String topic) {
        byte[] encodedPayload = new byte[0];
        try {
            encodedPayload = payload.getBytes("UTF-8");
            MqttMessage message = new MqttMessage(encodedPayload);
            message.setRetained(true);
            client.publish(topic, message);
        } catch (UnsupportedEncodingException | MqttException e) {
            e.printStackTrace();
        }
    }

    public void subscribe(MqttAndroidClient client, String topic) {
        int qos = 1;
        try {
            IMqttToken subToken = client.subscribe(topic, qos);
            subToken.setActionCallback(new IMqttActionListener() {

                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void findView() {
        btn = (Button) findViewById(R.id.btn_save);
        tvSum = (TextView) findViewById(R.id.tv_sum);
        tvCurrMonth = (TextView) findViewById(R.id.tv_current_month);
        tvPreMonth = (TextView) findViewById(R.id.tv_previous_month);
        tvEltCurr = (TextView) findViewById(R.id.tv_current_electric);
        tvWtCurr = (TextView) findViewById(R.id.tv_current_water);
        tvEltPre = (TextView) findViewById(R.id.tv_previous_electric);
        tvWtPre = (TextView) findViewById(R.id.tv_previous_water);
        btnOnOff = (ToggleButton) findViewById(R.id.btn_on_off);
        rdgSelect = (RadioGroup) findViewById(R.id.rdg_select_mode);
        etRoom = (EditText) findViewById(R.id.et_room);
        etTime = (EditText) findViewById(R.id.et_time);
        btnLoadData = (Button) findViewById(R.id.btn_load_data);
    }

    public String[] sliptString(String s) {
        String[] result = new String[2];
        String[] W = s.split("E");
        String[] E = W[1].split("W");
        result[0] = E[0];
        result[1] = W[0];
        return result;
    }

    public long calculateSum() {
        long water = Long.parseLong(tvWtCurr.getText().toString()) - Long.parseLong(tvWtPre.getText().toString());
        long elec = Long.parseLong(tvEltCurr.getText().toString()) - Long.parseLong(tvEltPre.getText().toString());
        return water * WATER_PRICE + elec * ELEC_PRICE + ROOM_PRICE;
    }
}
