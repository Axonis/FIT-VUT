<?php

namespace App\Http\Controllers;

use App\DataTypes;
use Illuminate\Http\Request;
use App\User;
use DB;

class AdminController extends Controller
{
    public function index(){
        $users = User::where('group', 'User')->orWhere('group', 'Programmer')->get();
        return view('admin.admin', compact('users'));
    }

    public function show($name){
        $profile = User::where('name', $name)->get();
        $produkty = DataTypes::$produkt;

        if($profile[0]->group === 'Programmer') {
            $aux = DB::table('programmers')->where('name', $name)->get();
        }
        else {
            $aux = DB::table('bfu')->where('name', $name)->get();
        }

        $profile = $profile[0];
        $aux = $aux[0];

        return view('admin.profile', compact('aux', 'profile', 'produkty'));
    }

    public function store(Request $request){
        if($request->rank and $request->produkt and $request->karma) {
            DB::table('bfu')->where('name', $request->name)->delete();
            DB::table('programmers')->insert([
                'name' => $request->name,
                'rank' => $request->rank,
                'produkt' => $request->produkt
            ]);
            DB::table('users')->where('name', $request->name)->update([
                'group' => 'Programmer'
            ]);
        }
        if($request->rank and $request->produkt and !$request->karma) {
            DB::table('programmers')->update([
                'name' => $request->name,
                'rank' => $request->rank,
                'produkt' => $request->produkt
            ]);
        }
           return redirect('/admin/'.$request->name);


    }
}
