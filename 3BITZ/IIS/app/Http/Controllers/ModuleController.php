<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Module;
use App\Language;
use App\User;
use DB;

class ModuleController extends Controller
{
    public function index(){
        $modules = Module::join('languages', 'id_lan' , '=', 'id')->get();
        return view('modules.modules', compact('modules'));
    }

    public function show($id){
        $module = Module::where('id_modul', $id)->join('languages', 'id_lan', '=', 'id')->first();
        $user = User::where('name', $module->name_zodpovedny)->first();
        $alt_users = DB::table('users')->join('know', 'name', '=', 'user')->
        where('id_lang', $module->id_lan)->get();
        return view('modules.module', compact('module', 'user', 'alt_users'));
    }

    public function store(Request $request){
        DB::table('modules')->where('id_modul', $request->id)->update([
            'chybovost' => $request->error,
            'name_zodpovedny' => $request->responsible,
        ]);

        return redirect('/modules/'.$request->id);    }

    public function create_show(){
        $langs = Language::all();
        $users = User::all();
        return view('modules.create', compact('langs', 'users'));
    }

    public function create_store(Request $request){
        $request->validate([
            'name' => 'required',
        ]);

        $lang = Language::where('name', $request->lang)->first();

        DB::table('modules')->insert([
            'id_lan'=> $lang->id,
            'meno' => $request->name,
            'chybovost' => 0.0,
            'name_zodpovedny' => $request->user,
        ]);

        $new_module = Module::all()->last();
        return redirect('/modules/'.$new_module->id_modul);
    }


}
