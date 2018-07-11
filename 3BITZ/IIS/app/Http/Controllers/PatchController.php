<?php

namespace App\Http\Controllers;

use App\Patch;
use App\Test;
use App\Http\Controllers\Auth;
use Illuminate\Http\Request;
use DB;

class PatchController extends Controller
{
    public function index()
    {
        $patches = Patch::all();
        return view('patches.patches', compact('patches'));
    }

    public function show($id_patch){
        $patch = Patch::where('id_patch', $id_patch)->first();
        if ($patch) {
            $tests = Test::where('id_patch', $patch->id_patch)->get();
        }
            return view('patches.patch', compact('patch', 'tests'));
    }

    public function store(Request $request){
        $cur_date = date("Y/m/d");
        if ($request->approve) {
            DB::table('patches')->where('id_patch', $request->id)->update([
                'name_schvaleny' => $request->approve,
                'datum_zavedenia' => $cur_date,
                'schvalenie' => 1,
            ]);
        }
        else {
            DB::table('patches')->where('id_patch', $request->id)->update([
                'datum_zavedenia' => $request->datum_zavedenia,
            ]);
        }

        return redirect('patches/'.$request->id);
    }

    public function create_show(){
        $bugs = DB::table('bugs')->join('modules', 'bugs.id_modul', '=', 'modules.id_modul')
            ->where('id_patch', null)->get();
        return view('patches.create', compact('bugs'));
    }

    public function create_store(Request $request){
        $validate = $request->validate([
            'diff' => 'required',
        ]);

        $date = date('Y/m/d');
        DB::table('patches')->insert([
            'datum_vydania' => $date,
            'schvalenie' => false,
            'name_vydany' => $request->user,
            'diff' => $request->diff,
        ]);

        $id_patch = DB::table('patches')->orderBy('id_patch', 'desc')->first()->id_patch;

        $bugs = explode(',', $request->id_bug);
        foreach ($bugs as $bug) {
            DB::table('bugs')->where('id_bug', $bug)->update([
                'id_patch' => $id_patch,
            ]);
        }

        $patch = Patch::all()->last();
        $route = '/patches/'.$patch->id_patch;
        return redirect($route);
    }
}
