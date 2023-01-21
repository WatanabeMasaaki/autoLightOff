//
//  MainView.swift
//  autoLightOff
//
//  Created by 渡邉雅晃 on 2022/12/22.
//

import SwiftUI

struct MainView: View {
    @ObservedObject var bleModel = BLEModel()
    @AppStorage("setHour") var setHour = 23
    @AppStorage("setMin") var setMin = 1
    
    var body: some View {
        VStack{
            Text("OFF時刻")
                .font(.title)
                .bold()
            
            Text(bleModel.connectStateLabel)
            
            
            Button(action: {
                bleModel.scan()
            }, label: {
                Text("接続する")
                    .font(.title)
            })
            .disabled(!bleModel.isDisabledConnectButton)
            
            Button(action: {
                bleModel.disconnectPeripheral()
            }, label: {
                Text("切断する")
                    .font(.title)
            })
            .disabled(!bleModel.isDisabledDisConnectButton)
            
            NavigationLink(destination: SettingView(bleModel: bleModel, hour: setHour, min: setMin), label: {
                VStack{
                    let setMinStr = setMin < 10 ? "0" + String(setMin) : String(setMin)
                    Text("\(setHour):\(setMinStr)  ")
                        .foregroundColor(.white)
                        .font(.system(size:128))
                        .background(Color.blue.cornerRadius(40))
                        
                    Text("設定")
                        .font(.title)
                }
            })
            .disabled(!bleModel.isDisabledWriteButton)
        }
    }
}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        MainView()
    }
}
