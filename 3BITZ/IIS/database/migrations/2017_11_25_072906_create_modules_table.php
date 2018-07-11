<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreateModulesTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('modules', function (Blueprint $table) {
            $table->increments('id_modul');
            $table->string('meno');
            $table->float('chybovost');
            $table->unsignedInteger('id_lan');
            $table->string('name_zodpovedny');
            $table->foreign('name_zodpovedny')->references('name')->on('users');
            $table->foreign('id_lan')->references('id')->on('languages');
        });


    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('modules');
    }
}
